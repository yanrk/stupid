/********************************************************
 * Description : thread group class
 * Data        : 2015-07-02 09:57:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_THREAD_GROUP_H
#define STUPID_BASE_THREAD_GROUP_H


#include <list>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/locker/locker.h"
#include "base/thread/thread.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API ThreadGroup : private Uncopy
{
public:
    ThreadGroup();
    ~ThreadGroup();

public:
    size_t size();
    bool acquire_thread(thread_func_ptr_t func, void * param, const char * name = nullptr);
    void release_threads();

private:
    std::list<Thread *>     m_thread_list;
    ThreadLocker            m_thread_locker;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_THREAD_GROUP_H
