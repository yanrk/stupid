/********************************************************
 * Description : process class
 * Data        : 2016-09-09 10:13:16
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_PROCESS_H
#define STUPID_BASE_PROCESS_H


#include <string>
#include <vector>
#include "base/common/common.h"

#ifdef _MSC_VER
    #include "base/process/windows_process.h"
#else
    #include "base/process/unix_process.h"
#endif // _MSC_VER

NAMESPACE_STUPID_BASE_BEGIN

#ifdef _MSC_VER
    typedef WindowsJoinProcess   Process;
#else
    typedef UnixJoinProcess      Process;
#endif // _MSC_VER

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_PROCESS_H
