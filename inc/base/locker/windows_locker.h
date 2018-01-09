/********************************************************
 * Description : locker classes in windows
 * Data        : 2013-05-20 09:02:33
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_WINDOWS_LOCKER_H
#define STUPID_BASE_WINDOWS_LOCKER_H


#ifdef _MSC_VER


#include <windows.h>

#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API WindowsThreadLocker : private Uncopy
{
public:
    WindowsThreadLocker(const char * name = nullptr);
    ~WindowsThreadLocker();

public:
    bool acquire() const;
    void release() const;
    bool try_acquire() const; /* not support, alway be false */

private:
    std::string                m_name;
    mutable CRITICAL_SECTION   m_locker;
};


class STUPID_API WindowsProcessLocker : private Uncopy
{
public:
    WindowsProcessLocker(const char * name = nullptr);
    ~WindowsProcessLocker();

public:
    bool acquire() const;
    void release() const;
    bool try_acquire() const;

private:
    std::string      m_name;
    mutable HANDLE   m_locker;
};

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER


#endif // STUPID_BASE_WINDOWS_LOCKER_H
