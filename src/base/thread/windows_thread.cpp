/********************************************************
 * Description : thread class in windows
 * Data        : 2013-05-23 15:30:53
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifdef _MSC_VER


#include <process.h>

#include <cassert>
#include "base/utility/guard.h"
#include "base/thread/thread.h"

NAMESPACE_STUPID_BASE_BEGIN

bool stupid_create_detached_thread(thread_func_ptr_t func, thread_argument_t argu)
{
    HANDLE thread_handle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, argu, 0, nullptr));
    if (nullptr == thread_handle)
    {
        return(false);
    }
    else
    {
        CloseHandle(thread_handle);
        return(true);
    }
}

WindowsJoinThread::WindowsJoinThread(thread_func_ptr_t func, void * argument, const char * name)
    : m_func(func)
    , m_argu(argument)
    , m_name(nullptr != name ? name : "")
    , m_thread(nullptr)
    , m_running(false)
    , m_locker("thread locker")
{

}

WindowsJoinThread::~WindowsJoinThread()
{
    release();
}

bool WindowsJoinThread::acquire()
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (m_running || nullptr == m_func)
    {
        return(false);
    }
    m_running = true;

    m_thread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, m_func, m_argu, 0, nullptr));
    if (nullptr == m_thread)
    {
        m_running = false;
        return(false);
    }
    else
    {
        return(true);
    }
}

void WindowsJoinThread::release()
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running || nullptr == m_thread)
    {
        return;
    }
    m_running = false;

    WaitForSingleObject(m_thread, INFINITE);
    CloseHandle(m_thread);
}

void WindowsJoinThread::set_thread_args(thread_func_ptr_t func, void * argument, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_func = func;
        m_argu = argument;
        m_name = (nullptr != name ? name : "");
    }
}

bool WindowsJoinThread::running() const
{
    return(m_running);
}

size_t WindowsJoinThread::thread_id() const
{
    return(static_cast<size_t>(GetCurrentThreadId()));
}

std::string WindowsJoinThread::thread_name()
{
    return(m_name);
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER
