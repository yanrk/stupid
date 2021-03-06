/********************************************************
 * Description : locker classes
 * Data        : 2013-05-20 08:03:27
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_LOCKER_H
#define STUPID_BASE_LOCKER_H


#include "base/locker/null_locker.h"

#ifdef _MSC_VER
    #include "base/locker/windows_locker.h"
#else
    #include "base/locker/unix_locker.h"
#endif // _MSC_VER

NAMESPACE_STUPID_BASE_BEGIN

#ifdef _MSC_VER
    typedef WindowsThreadLocker      ThreadLocker;
    typedef WindowsProcessLocker     ProcessLocker;
#else
    typedef UnixMutexLocker<true>    ThreadLocker;
    typedef UnixMutexLocker<false>   ProcessLocker;
#endif // _MSC_VER

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_LOCKER_H
