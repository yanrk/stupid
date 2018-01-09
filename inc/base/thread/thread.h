/********************************************************
 * Description : thread class
 * Data        : 2013-05-23 18:02:15
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_THREAD_H
#define STUPID_BASE_THREAD_H


#include "base/common/common.h"

#ifdef _MSC_VER
    #include "base/thread/windows_thread.h"
#else
    #include "base/thread/unix_thread.h"
#endif // _MSC_VER

NAMESPACE_STUPID_BASE_BEGIN

#ifdef _MSC_VER
    typedef WindowsJoinThread   Thread;
#else
    typedef UnixJoinThread      Thread;
#endif // _MSC_VER

STUPID_CXX_API(bool) stupid_create_detached_thread(thread_func_ptr_t func, thread_argument_t argu);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_THREAD_H
