/********************************************************
 * Description : thread class in windows
 * Data        : 2013-05-23 15:30:53
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_WINDOWS_THREAD_H
#define STUPID_BASE_WINDOWS_THREAD_H


#ifdef _MSC_VER


#include <windows.h>

#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/locker/locker.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API WindowsJoinThread : private Uncopy
{
public:
    WindowsJoinThread(thread_func_ptr_t func = nullptr, void * argument = nullptr, const char * name = nullptr);
    ~WindowsJoinThread();

public:
    bool acquire();
    void release();

public:
    void set_thread_args(thread_func_ptr_t func, void * argument = nullptr, const char * name = nullptr);

public:
    bool running();
    std::string thread_name();
    size_t thread_id() const;

private:
    thread_func_ptr_t   m_func;
    void              * m_argu;
    std::string         m_name;
    HANDLE              m_thread;
    volatile bool       m_running;
    ThreadLocker        m_locker;
};

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER


#endif // STUPID_BASE_WINDOWS_THREAD_H
