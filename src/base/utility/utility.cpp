/********************************************************
 * Description : utility of base
 * Data        : 2015-02-28 10:38:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <unistd.h>
    #include <pthread.h>
#endif // _MSC_VER

#include "base/utility/utility.h"

NAMESPACE_STUPID_BASE_BEGIN

size_t get_pid()
{
#ifdef _MSC_VER
    return static_cast<size_t>(GetCurrentProcessId());
#else
    return static_cast<size_t>(getpid());
#endif // _MSC_VER
}

size_t get_tid()
{
#ifdef _MSC_VER
    return static_cast<size_t>(GetCurrentThreadId());
#elif defined(_MAC_OS)
    return reinterpret_cast<size_t>(pthread_self());
#else
    return static_cast<size_t>(pthread_self());
#endif // _MSC_VER
}

NAMESPACE_STUPID_BASE_END
