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

#ifndef _MSC_VER


#include "base/thread/linux_thread_local_storage.h"

NAMESPACE_STUPID_BASE_BEGIN

LinuxThreadLocalStorage::LinuxThreadLocalStorage()
    : m_thread_key(0)
{
    pthread_key_create(&m_thread_key, nullptr);
}

LinuxThreadLocalStorage::~LinuxThreadLocalStorage()
{
    pthread_key_delete(m_thread_key);
}

void LinuxThreadLocalStorage::set_value(void * thread_specific_value)
{
    pthread_setspecific(m_thread_key, thread_specific_value);
}

void * LinuxThreadLocalStorage::get_value()
{
    return(pthread_getspecific(m_thread_key));
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER
