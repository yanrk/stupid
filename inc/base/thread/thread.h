/********************************************************
 * Description : thread class
 * Data        : 2013-05-23 18:02:15
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_THREAD_H
#define STUPID_BASE_THREAD_H


#include "base/common/common.h"

#ifdef _MSC_VER
    #include "base/thread/windows_thread.h"
#else
    #include "base/thread/linux_thread.h"
#endif // _MSC_VER

NAMESPACE_STUPID_BASE_BEGIN

#ifdef _MSC_VER
    typedef WindowsJoinThread   Thread;
#else
    typedef LinuxJoinThread     Thread;
#endif // _MSC_VER

STUPID_CXX_API(bool) stupid_create_detached_thread(thread_func_ptr_t func, thread_argument_t argu);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_THREAD_H
