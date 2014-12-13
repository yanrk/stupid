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

#ifndef STUPID_BASE_NULL_LOCKER_H
#define STUPID_BASE_NULL_LOCKER_H


#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API NullLocker : private Uncopy
{
public:
    NullLocker(const char * name = nullptr);
    ~NullLocker();

public:
    bool acquire();
    void release();
    bool try_acquire();
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_NULL_LOCKER_H
