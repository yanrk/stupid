/********************************************************
 * Description : resource references counter
 * Data        : 2014-12-05 09:51:55
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_COUNTER_H
#define STUPID_BASE_COUNTER_H


#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class CounterImpl;

class STUPID_API Counter : private Uncopy
{
public:
    Counter(size_t count = 0);
    ~Counter();

public:
    void reset(size_t count = 0);

public:
    void increase();
    void decrease();

public:
    bool valid() const;
    size_t references() const;

private:
    CounterImpl * m_counter;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_COUNTER_H
