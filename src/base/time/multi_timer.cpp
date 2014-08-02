/********************************************************
 * Description : multi timer
 * Data        : 2013-08-09 00:28:45
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <algorithm>
#include "base/utility/guard.h"
#include "base/time/time.h"
#include "base/time/multi_timer.h"

NAMESPACE_STUPID_BASE_BEGIN

struct CompTimerWithID
{
    CompTimerWithID(size_t id)
        : m_id(id)
    {

    }

    bool operator () (ITimer * timer) const
    {
        return(m_id == timer->m_id);
    }

    size_t m_id;
};

struct CompTimerWithStartTime
{
    CompTimerWithStartTime(uint64_t start_time)
        : m_start_time(start_time)
    {

    }

    bool operator () (ITimer * timer) const
    {
        return(m_start_time < timer->m_start_time);
    }

    uint64_t   m_start_time;
};

static uint64_t diff_millisecond(struct timeval stime)
{
    uint64_t diff = 0;
    struct timeval etime = stupid_gettimeofday();
    diff += (etime.tv_sec - stime.tv_sec) * 1000;
    diff += (etime.tv_usec - stime.tv_usec) / 1000;
    return(diff);
}

static thread_return_t STUPID_STDCALL multi_timer_run(thread_argument_t argument)
{
    MultiTimer * multi_timer = reinterpret_cast<MultiTimer *>(argument);
    if (nullptr != multi_timer)
    {
        multi_timer->thread_run();
    }
    return(THREAD_DEFAULT_RET);
}

MultiTimer::MultiTimer()
    : m_running(false)
    , m_timer_id(BAD_TIMER_ID + 1)
    , m_current_time(0)
    , m_pending_timer()
    , m_running_timer()
    , m_id_locker()
    , m_pending_locker()
    , m_running_locker()
    , m_thread()
{
    m_thread.set_thread_args(multi_timer_run, this, "multi timer");
}

MultiTimer::~MultiTimer()
{
    exit();
}

bool MultiTimer::init()
{
    if (m_running)
    {
        return(true);
    }

    m_running = true;

    return(m_thread.acquire());
}

void MultiTimer::exit()
{
    if (!m_running)
    {
        return;
    }

    m_running = false;

    m_thread.release();

    m_timer_id = BAD_TIMER_ID + 1;

    std::list<ITimer *>::iterator pending_iter = m_pending_timer.begin();
    while (m_pending_timer.end() != pending_iter)
    {
        ITimer * timer = *pending_iter;
        delete timer;
        ++pending_iter;
    }
    m_pending_timer.clear();

    std::list<ITimer *>::iterator running_iter = m_running_timer.begin();
    while (m_running_timer.end() != running_iter)
    {
        ITimer * timer = *running_iter;
        delete timer;
        ++running_iter;
    }
    m_running_timer.clear();
}

bool MultiTimer::start_timer(size_t id, bool once, size_t period)
{
    if (!m_running)
    {
        return(false);
    }

    ITimer * timer = nullptr;
    Guard<ThreadLocker> pending_guard(m_pending_locker);
    std::list<ITimer *>::iterator iter = std::find_if(m_pending_timer.begin(), m_pending_timer.end(), CompTimerWithID(id));
    if (m_pending_timer.end() == iter)
    {
        return(false);
    }
    timer = *iter;
    m_pending_timer.erase(iter);
    pending_guard.release();

    timer->m_once = once;
    timer->m_period = period;
    timer->m_start_time = m_current_time + period;

    add_running_timer(timer);

    return(true);
}

bool MultiTimer::stop_timer(size_t id)
{
    if (!m_running)
    {
        return(false);
    }

    ITimer * timer = nullptr;
    Guard<ThreadLocker> running_guard(m_running_locker);
    std::list<ITimer *>::iterator iter = std::find_if(m_running_timer.begin(), m_running_timer.end(), CompTimerWithID(id));
    if (m_running_timer.end() == iter)
    {
        return(false);
    }
    timer = *iter;
    m_running_timer.erase(iter);
    running_guard.release();

    add_pending_timer(timer);

    return(true);
}

void MultiTimer::destroy_timer(size_t id)
{
    if (!m_running)
    {
        return;
    }

    /* the order of function invoking is important */
    ITimer * running_timer = del_running_timer(id);
    STUPID_DEL(running_timer);
    ITimer * pending_timer = del_pending_timer(id);
    STUPID_DEL(pending_timer);
}

bool MultiTimer::running() const
{
    return(m_running);
}

void MultiTimer::thread_run()
{
    struct timeval stime = stupid_gettimeofday();
    std::list<ITimer *> timer_list;

    while (running())
    {
        Guard<ThreadLocker> running_guard(m_running_locker);
        while (!m_running_timer.empty())
        {
            ITimer * timer = m_running_timer.front();
            if (timer->m_start_time > m_current_time)
            {
                break;
            }
            m_running_timer.pop_front();

            timer->doit();
            timer->m_start_time = m_current_time + timer->m_period;
            timer_list.push_back(timer);
        }

        std::list<ITimer *>::iterator iter = timer_list.begin();
        while (timer_list.end() != iter)
        {
            ITimer * timer = *iter;
            if (timer->m_once)
            {
                add_pending_timer(*iter);
            }
            else
            {
                add_running_timer_unlock(*iter);
            }
            ++iter;
        }
        timer_list.clear();
        running_guard.release();

        stupid_ms_sleep(1);

        m_current_time = diff_millisecond(stime);
    }
}

size_t MultiTimer::acquire_timer_id()
{
    size_t id = BAD_TIMER_ID;
    Guard<ThreadLocker> id_guard(m_id_locker);
    if (BAD_TIMER_ID != m_timer_id)
    {
        id = m_timer_id++;
    }
    id_guard.release();

    return(id);
}

void MultiTimer::add_pending_timer(ITimer * timer)
{
    Guard<ThreadLocker> pending_guard(m_pending_locker);
    m_pending_timer.push_back(timer);
    pending_guard.release();
}

ITimer * MultiTimer::del_pending_timer(size_t id)
{
    ITimer * timer = nullptr;

    Guard<ThreadLocker> pending_guard(m_pending_locker);
    std::list<ITimer *>::iterator iter = std::find_if(m_pending_timer.begin(), m_pending_timer.end(), CompTimerWithID(id));
    if (m_pending_timer.end() != iter)
    {
        timer = *iter;
        m_pending_timer.erase(iter);
    }
    pending_guard.release();

    return(timer);
}

void MultiTimer::add_running_timer(ITimer * timer)
{
    Guard<ThreadLocker> running_guard(m_running_locker);
    add_running_timer_unlock(timer);
    running_guard.release();
}

void MultiTimer::add_running_timer_unlock(ITimer * timer)
{
    std::list<ITimer *>::iterator iter = std::find_if(m_running_timer.begin(), m_running_timer.end(), CompTimerWithStartTime(timer->m_start_time));
    m_running_timer.insert(iter, timer);
}

ITimer * MultiTimer::del_running_timer(size_t id)
{
    ITimer * timer = nullptr;

    Guard<ThreadLocker> running_guard(m_running_locker);
    std::list<ITimer *>::iterator iter = std::find_if(m_running_timer.begin(), m_running_timer.end(), CompTimerWithID(id));
    if (m_running_timer.end() != iter)
    {
        timer = *iter;
        m_running_timer.erase(iter);
    }
    running_guard.release();

    return(timer);
}

NAMESPACE_STUPID_BASE_END
