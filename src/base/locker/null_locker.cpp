/********************************************************
 * Description : null locker class
 * Data        : 2013-05-30 16:45:57
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "base/locker/null_locker.h"

NAMESPACE_STUPID_BASE_BEGIN

NullLocker::NullLocker(const char * name)
{
    name = nullptr; /* prevent warning */
}

NullLocker::~NullLocker()
{

}

bool NullLocker::acquire() const
{
    return true;
}

void NullLocker::release() const
{

}

bool NullLocker::try_acquire() const
{
    return true;
}

NAMESPACE_STUPID_BASE_END
