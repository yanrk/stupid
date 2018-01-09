/********************************************************
 * Description : thread class in unix
 * Data        : 2013-05-23 17:03:46
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
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

UnixJoinThread::UnixJoinThread(thread_func_ptr_t func, void * argument, const char * name)
    : m_func(func)
    , m_argu(argument)
    , m_name(nullptr != name ? name : "")
    , m_thread()
    , m_running(false)
    , m_locker("thread locker")
{

}

UnixJoinThread::~UnixJoinThread()
{
    release();
}

bool UnixJoinThread::acquire()
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

void UnixJoinThread::release()
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        return;
    }
    m_running = false;

    pthread_join(m_thread, nullptr);
}

void UnixJoinThread::set_thread_args(thread_func_ptr_t func, void * argument, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_func = func;
        m_argu = argument;
        m_name = (nullptr != name ? name : "");
    }
}

bool UnixJoinThread::running()
{
    return(m_running);
}

size_t UnixJoinThread::thread_id() const
{
#ifdef _MAC_OS
    return(reinterpret_cast<size_t>(pthread_self()));
#else
    return(static_cast<size_t>(pthread_self()));
#endif // _MAC_OS
}

std::string UnixJoinThread::thread_name()
{
    return(m_name);
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER
