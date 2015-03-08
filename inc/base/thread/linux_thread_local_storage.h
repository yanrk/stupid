/********************************************************
 * Description : thread local storage class in linux
 * Data        : 2015-02-27 15:23:00
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_LINUX_THREAD_LOCAL_STORAGE_H
#define STUPID_BASE_LINUX_THREAD_LOCAL_STORAGE_H


#ifndef _MSC_VER


#include <pthread.h>

#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API LinuxThreadLocalStorage : private Uncopy
{
public:
    LinuxThreadLocalStorage();
    ~LinuxThreadLocalStorage();

public:
    void set_value(void * thread_specific_value);
    void * get_value();

private:
    pthread_key_t               m_thread_key;
};

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER


#endif // STUPID_BASE_LINUX_THREAD_LOCAL_STORAGE_H
