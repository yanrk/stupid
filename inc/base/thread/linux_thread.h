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

#ifndef STUPID_BASE_LINUX_THREAD_H
#define STUPID_BASE_LINUX_THREAD_H


#ifndef _MSC_VER


#include <pthread.h>

#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/locker/locker.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API LinuxJoinThread : private Uncopy
{
public:
    LinuxJoinThread(thread_func_ptr_t func = nullptr, void * argument = nullptr, const char * name = nullptr);
    ~LinuxJoinThread();

public:
    bool acquire();
    void release();

public:
    void set_thread_args(thread_func_ptr_t func, void * argument = nullptr, const char * name = nullptr);

public:
    bool running() const;
    std::string thread_name();
    size_t thread_id() const;

private:
    thread_func_ptr_t   m_func;
    void              * m_argu;
    std::string         m_name;
    pthread_t           m_thread;
    bool                m_running;
    ThreadLocker        m_locker;
};

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER


#endif // STUPID_BASE_LINUX_THREAD_H
