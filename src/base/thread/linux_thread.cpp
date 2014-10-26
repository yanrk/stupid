/********************************************************
 * Description : thread class in linux
 * Data        : 2013-05-23 17:03:46
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef _MSC_VER


#include <cassert>
#include "base/utility/guard.h"
#include "base/thread/thread.h"

NAMESPACE_STUPID_BASE_BEGIN

bool stupid_create_detached_thread(thread_func_ptr_t func, thread_argument_t argu)
{
    int              error_code = 0;
    pthread_t        thread_id;
    pthread_attr_t   thread_attr;

    error_code = pthread_attr_init(&thread_attr);
    if (0 != error_code)
    {
        return(false);
    }

    do
    {
        error_code = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
        if (0 != error_code)
        {
            break;
        }

        error_code = pthread_create(&thread_id, &thread_attr, func, argu);
        if (0 != error_code)
        {
            break;
        }
    } while (false);

    pthread_attr_destroy(&thread_attr);

    return(0 == error_code);
}

LinuxJoinThread::LinuxJoinThread(thread_func_ptr_t func, void * argument, const char * name)
    : m_func(func)
    , m_argu(argument)
    , m_name(nullptr != name ? name : "")
    , m_thread()
    , m_running(false)
    , m_locker("thread locker")
{

}

LinuxJoinThread::~LinuxJoinThread()
{
    release();
}

bool LinuxJoinThread::acquire()
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (m_running || nullptr == m_func)
    {
        return(false);
    }
    m_running = true;

    if (0 != pthread_create(&m_thread, nullptr, m_func, m_argu))
    {
        m_running = false;
        return(false);
    }
    else
    {
        return(true);
    }
}

void LinuxJoinThread::release()
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        return;
    }
    m_running = false;

    pthread_join(m_thread, nullptr);
}

void LinuxJoinThread::set_thread_args(thread_func_ptr_t func, void * argument, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_func = func;
        m_argu = argument;
        m_name = (nullptr != name ? name : "");
    }
}

bool LinuxJoinThread::running()
{
    return(m_running);
}

size_t LinuxJoinThread::thread_id() const
{
    return(static_cast<size_t>(pthread_self()));
}

std::string LinuxJoinThread::thread_name()
{
    return(m_name);
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER
