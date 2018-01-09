/********************************************************
 * Description : thread local storage class
 * Data        : 2015-02-27 16:18:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_THREAD_LOCAL_STORAGE_H
#define STUPID_BASE_THREAD_LOCAL_STORAGE_H


#include "base/common/common.h"

#ifdef _MSC_VER
    #include "base/thread/windows_thread_local_storage.h"
#else
    #include "base/thread/unix_thread_local_storage.h"
#endif // _MSC_VER

NAMESPACE_STUPID_BASE_BEGIN

#ifdef _MSC_VER
    typedef WindowsThreadLocalStorage   ThreadLocalStorage;
#else
    typedef UnixThreadLocalStorage      ThreadLocalStorage;
#endif // _MSC_VER

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_THREAD_LOCAL_STORAGE_H
