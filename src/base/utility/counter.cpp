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

#include "base/locker/locker.h"
#include "base/utility/guard.h"
#include "base/utility/uncopy.h"
#include "base/utility/counter.h"

NAMESPACE_STUPID_BASE_BEGIN

class CounterImpl : private Uncopy
{
public:
    CounterImpl(size_t count);
    ~CounterImpl();

public:
    void reset(size_t count);

public:
    void increase();
    void decrease();

public:
    size_t references() const;

private:
    volatile size_t   m_count;
    ThreadLocker      m_locker;
};

CounterImpl::CounterImpl(size_t count)
    : m_count(count)
    , m_locker()
{

}

CounterImpl::~CounterImpl()
{

}

void CounterImpl::reset(size_t count)
{
    Guard<ThreadLocker> guard(m_locker);
    m_count = count;
}

void CounterImpl::increase()
{
    Guard<ThreadLocker> guard(m_locker);
    ++m_count;
}

void CounterImpl::decrease()
{
    Guard<ThreadLocker> guard(m_locker);
    --m_count;
}

size_t CounterImpl::references() const
{
    return m_count;
}

Counter::Counter(size_t count)
    : m_counter(new CounterImpl(count))
{

}

Counter::~Counter()
{
    delete m_counter;
}

void Counter::reset(size_t count)
{
    if (nullptr != m_counter)
    {
        m_counter->reset(count);
    }
}

void Counter::increase()
{
    if (nullptr != m_counter)
    {
        m_counter->increase();
    }
}

void Counter::decrease()
{
    if (nullptr != m_counter)
    {
        m_counter->decrease();
    }
}

bool Counter::valid() const
{
    return nullptr != m_counter;
}

size_t Counter::references() const
{
    if (nullptr != m_counter)
    {
        return m_counter->references();
    }
    else
    {
        return ~static_cast<size_t>(0);
    }
}

NAMESPACE_STUPID_BASE_END
