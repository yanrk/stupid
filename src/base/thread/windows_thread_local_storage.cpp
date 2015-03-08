/********************************************************
 * Description : thread local storage class in windows
 * Data        : 2015-02-27 16:01:00
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifdef _MSC_VER


#include <cassert>
#include "base/thread/windows_thread_local_storage.h"

NAMESPACE_STUPID_BASE_BEGIN

WindowsThreadLocalStorage::WindowsThreadLocalStorage()
    : m_thread_key(TlsAlloc())
{
    assert(TLS_OUT_OF_INDEXES != m_thread_key);
}

WindowsThreadLocalStorage::~WindowsThreadLocalStorage()
{
    TlsFree(m_thread_key);
}

void WindowsThreadLocalStorage::set_value(void * thread_specific_value)
{
    TlsSetValue(m_thread_key, thread_specific_value);
}

void * WindowsThreadLocalStorage::get_value()
{
    return(TlsGetValue(m_thread_key));
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER
