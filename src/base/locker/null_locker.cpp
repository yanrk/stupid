/********************************************************
 * Description : null locker class
 * Data        : 2013-05-30 16:45:57
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
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

bool NullLocker::acquire()
{
    return(true);
}

void NullLocker::release()
{

}

bool NullLocker::try_acquire()
{
    return(true);
}

NAMESPACE_STUPID_BASE_END
