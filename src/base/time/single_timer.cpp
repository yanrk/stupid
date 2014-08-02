/********************************************************
 * Description : single timer
 * Data        : 2013-08-09 22:21:48
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include "base/time/time.h"
#include "base/time/single_timer.h"

NAMESPACE_STUPID_BASE_BEGIN

static thread_return_t STUPID_STDCALL single_timer_run(thread_argument_t argument)
{
    SingleTimer * timer = reinterpret_cast<SingleTimer *>(argument);
    if (nullptr != timer)
    {
        SingleTimer::run(timer);
    }
    return(THREAD_DEFAULT_RET);
}

ISingleTimerSink::~ISingleTimerSink()
{

}

SingleTimer::SingleTimer()
    : m_sink(nullptr)
    , m_period(0)
    , m_index(0)
    , m_first_time(true)
    , m_running(false)
    , m_thread()
{
    m_thread.set_thread_args(single_timer_run, this, "single timer");
}

SingleTimer::~SingleTimer()
{
    exit();
}

bool SingleTimer::init(ISingleTimerSink * sink, size_t period)
{
    exit();

    if (nullptr == sink)
    {
        return(false);
    }

    m_sink = sink;
    m_period = period;
    m_index = 0;
    m_first_time = true;
    m_running = true;

    if (!m_thread.acquire())
    {
        m_running = false;
    }

    return(m_running);
}

void SingleTimer::exit()
{
    if (m_running)
    {
        m_running = false;
        m_thread.release();
    }
}

bool SingleTimer::running() const
{
    return(m_running);
}

void SingleTimer::run(SingleTimer * timer)
{
    if (nullptr == timer)
    {
        return;
    }

    while (timer->running())
    {
        timer->m_sink->on_timer(timer->m_first_time, timer->m_index);
        stupid_ms_sleep(timer->m_period);
        ++timer->m_index;
        timer->m_first_time = false;
    }
}

NAMESPACE_STUPID_BASE_END
