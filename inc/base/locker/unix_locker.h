/********************************************************
 * Description : locker classes in unix
 * Data        : 2013-05-20 10:46:27
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_UNIX_LOCKER_H
#define STUPID_BASE_UNIX_LOCKER_H


#ifndef _MSC_VER


#include <pthread.h>

#include <cassert>
#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

template <bool b_thread_lock>
class UnixMutexLocker : private Uncopy
{
public:
    UnixMutexLocker(const char * name = nullptr);
    ~UnixMutexLocker();

public:
    bool acquire() const;
    void release() const;
    bool try_acquire() const;

private:
    std::string               m_name;
    mutable pthread_mutex_t   m_locker;
};

template <bool b_thread_lock>
UnixMutexLocker<b_thread_lock>::UnixMutexLocker(const char * name)
    : m_name(nullptr != name ? name : "")
    , m_locker()
{
    pthread_mutexattr_t locker_attr;

    if (0 != pthread_mutexattr_init(&locker_attr))
    {
        assert(false);
    }

    int shared_state = b_thread_lock ? PTHREAD_PROCESS_PRIVATE : PTHREAD_PROCESS_SHARED;
    if (0 != pthread_mutexattr_setpshared(&locker_attr, shared_state))
    {
        assert(false);
    }

    if (0 != pthread_mutex_init(&m_locker, &locker_attr))
    {
        assert(false);
    }

    if (0 != pthread_mutexattr_destroy(&locker_attr))
    {
        assert(false);
    }
}

template <bool b_thread_lock>
UnixMutexLocker<b_thread_lock>::~UnixMutexLocker()
{
    pthread_mutex_destroy(&m_locker);
}

template <bool b_thread_lock>
bool UnixMutexLocker<b_thread_lock>::acquire() const
{
    return 0 == pthread_mutex_lock(&m_locker);
}

template <bool b_thread_lock>
void UnixMutexLocker<b_thread_lock>::release() const
{
    pthread_mutex_unlock(&m_locker);
}

template <bool b_thread_lock>
bool UnixMutexLocker<b_thread_lock>::try_acquire() const
{
    return 0 == pthread_mutex_trylock(&m_locker);
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER


#endif // STUPID_BASE_UNIX_LOCKER_H
