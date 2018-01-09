/********************************************************
 * Description : thread local storage class in windows
 * Data        : 2015-02-27 16:01:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_WINDOWS_THREAD_LOCAL_STORAGE_H
#define STUPID_BASE_WINDOWS_THREAD_LOCAL_STORAGE_H


#ifdef _MSC_VER


#include <windows.h>

#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API WindowsThreadLocalStorage : private Uncopy
{
public:
    WindowsThreadLocalStorage();
    ~WindowsThreadLocalStorage();

public:
    void set_value(void * thread_specific_value);
    void * get_value();

private:
    DWORD                   m_thread_key;
};

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER


#endif // STUPID_BASE_WINDOWS_THREAD_LOCAL_STORAGE_H
