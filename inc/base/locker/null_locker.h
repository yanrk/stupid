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
    bool acquire() const;
    void release() const;
    bool try_acquire() const;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_NULL_LOCKER_H
