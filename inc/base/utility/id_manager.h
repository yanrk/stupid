/********************************************************
 * Description : manager of unique identification
 * Data        : 2013-08-10 20:08:15
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_ID_MANAGER_H
#define STUPID_BASE_ID_MANAGER_H


#include <deque>
#include <algorithm>
#include <functional>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/utility/guard.h"
#include "base/locker/locker.h"

NAMESPACE_STUPID_BASE_BEGIN

template <typename IDType, typename LockerType = NullLocker>
class IDManager : private Uncopy
{
public:
    IDManager(IDType min, IDType max);

public:
    void reset();

public:
    bool acquire(IDType & id);
    bool release(IDType id);

private:
    const IDType         m_min_id;
    const IDType         m_max_id;
    IDType               m_next_id;
    std::deque<IDType>   m_unused_id;
    LockerType           m_locker;
};

template <typename IDType, typename LockerType>
IDManager<IDType, LockerType>::IDManager(IDType min, IDType max)
    : m_min_id(min)
    , m_max_id(max)
    , m_next_id(min)
    , m_unused_id()
    , m_locker()
{

}

template <typename IDType, typename LockerType>
void IDManager<IDType, LockerType>::reset()
{
    Guard<LockerType> guard(m_locker);

    m_next_id = m_min_id;
    m_unused_id.clear();
}

template <typename IDType, typename LockerType>
bool IDManager<IDType, LockerType>::acquire(IDType & id)
{
    Guard<LockerType> guard(m_locker);

    if (!m_unused_id.empty())
    {
        id = m_unused_id.back();
        m_unused_id.pop_back();
        return(true);
    }

    if (m_next_id >= m_max_id)
    {
        return(false);
    }

    id = m_next_id++;

    return(true);
}

template <typename IDType, typename LockerType>
bool IDManager<IDType, LockerType>::release(IDType id)
{
    Guard<LockerType> guard(m_locker);

    if (id < m_min_id || id >= m_next_id)
    {
        return(false);
    }

    if (m_unused_id.end() != std::find(m_unused_id.begin(), m_unused_id.end(), id))
    {
        return(false);
    }

    if (id + 1 == m_next_id)
    {
        m_next_id = id;
    }
    else
    {
        m_unused_id.push_back(id);
        std::sort(m_unused_id.begin(), m_unused_id.end(), std::greater<IDType>());
    }

    typename std::deque<IDType>::iterator iter = m_unused_id.begin();
    while (m_unused_id.end() != iter)
    {
        if (*iter + 1 == m_next_id)
        {
            m_next_id = *iter++;
        }
        else
        {
            break;
        }
    }
    m_unused_id.erase(m_unused_id.begin(), iter);

    return(true);
}

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_ID_MANAGER_H
