/********************************************************
 * Description : thread local storage class in unix
 * Data        : 2015-02-27 15:23:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef _MSC_VER


#include "base/thread/unix_thread_local_storage.h"

NAMESPACE_STUPID_BASE_BEGIN

UnixThreadLocalStorage::UnixThreadLocalStorage()
    : m_thread_key(0)
{
    pthread_key_create(&m_thread_key, nullptr);
}

UnixThreadLocalStorage::~UnixThreadLocalStorage()
{
    pthread_key_delete(m_thread_key);
}

void UnixThreadLocalStorage::set_value(void * thread_specific_value)
{
    pthread_setspecific(m_thread_key, thread_specific_value);
}

void * UnixThreadLocalStorage::get_value()
{
    return(pthread_getspecific(m_thread_key));
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER
