/********************************************************
 * Description : memory pool class template
 * Data        : 2013-06-18 11:47:56
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_MEMORY_POOL_H
#define STUPID_BASE_MEMORY_POOL_H


#include <cassert>
#include <vector>
#include <list>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/utility/guard.h"
#include "base/locker/locker.h"

NAMESPACE_STUPID_BASE_BEGIN

template <typename LockerType = NullLocker>
class MemoryPool : private Uncopy
{
public:
    struct MemoryNode : private Uncopy
    {
        const size_t   index;
        const size_t   capacity;
        size_t         length;
        char           buffer[1];

        MemoryNode(size_t idx, size_t cap);
        ~MemoryNode();
    };

public:
    struct MemoryList
    {
        std::list<MemoryNode *>   m_list;
        LockerType                m_locker;
    };

public:
    MemoryPool();
    ~MemoryPool();

public:
    void clear();

public:
    MemoryNode * acquire(size_t length);
    void release(MemoryNode *& memory_node);

private:
    const size_t                     m_block_kinds;
    const size_t                     m_block_min_size;
    MemoryList                     * m_memory;
};

template <typename LockerType>
MemoryPool<LockerType>::MemoryNode::MemoryNode(size_t idx, size_t cap)
    : index(idx), capacity(cap), length(0)
{

}

template <typename LockerType>
MemoryPool<LockerType>::MemoryNode::~MemoryNode()
{

}

template <typename LockerType>
MemoryPool<LockerType>::MemoryPool()
    : m_block_kinds(14)
    , m_block_min_size(8)
    , m_memory(nullptr)
{
    STUPID_NEW_ARR(m_memory, MemoryList, m_block_kinds);
    assert(nullptr != m_memory);
}

template <typename LockerType>
MemoryPool<LockerType>::~MemoryPool()
{
    clear();
    STUPID_DEL_ARR(m_memory);
}

template <typename LockerType>
void MemoryPool<LockerType>::clear()
{
    for (size_t index = 0; index < m_block_kinds; ++index)
    {
        MemoryList & memory = m_memory[index];
        Guard<LockerType> memory_guard(memory.m_locker);
        std::list<MemoryNode *> & memory_node_list = memory.m_list;
        std::list<MemoryNode *>::iterator iter = memory_node_list.begin();
        while (memory_node_list.end() != iter)
        {
            MemoryNode * memory_node = *iter;
            memory_node->~MemoryNode();
            ++iter;
            char * buffer = reinterpret_cast<char *>(memory_node);
            STUPID_DEL_ARR(buffer);
        }
        memory_node_list.clear();
    }
}

template <typename LockerType>
MemoryNode * MemoryPool<LockerType>::acquire(size_t length)
{
    length += sizeof(MemoryNode);

    size_t index = 0;
    size_t adjust_size = m_block_min_size;
    while (adjust_size < length)
    {
        index += 1;
        adjust_size <<= 1;
    }

    if (index < m_block_kinds)
    {
        MemoryList & memory = m_memory[index];
        Guard<LockerType> memory_guard(memory.m_locker);
        std::list<MemoryNode *> & memory_node_list = memory.m_list;
        if (!memory_node_list.empty())
        {
            MemoryNode * memory_node = memory_node_list.front();
            memory_node_list.pop_front();
            return(memory_node);
        }
    }

    MemoryNode * memory_node = nullptr;
    char * buffer = nullptr;
    STUPID_NEW_ARR(buffer, char, adjust_size);
    if (nullptr != buffer)
    {
        new (buffer) MemoryNode(index, adjust_size);
        memory_node = reinterpret_cast<MemoryNode *>(buffer);
    }
    return(memory_node);
}

template <typename LockerType>
void MemoryPool<LockerType>::release(MemoryNode *& memory_node)
{
    if (nullptr == memory_node)
    {
        return;
    }

    memory_node->length = 0;

    if (memory_node->index >= m_block_kinds)
    {
        memory_node->~MemoryNode();
        char * buffer = reinterpret_cast<char *>(memory_node);
        STUPID_DEL_ARR(buffer);
        memory_node = nullptr;
        return;
    }

    MemoryList & memory = m_memory[memory_node->index];
    Guard<LockerType> memory_guard(memory.m_locker);
    std::list<MemoryNode *> & memory_node_list = memory.m_list;
    memory_node_list.push_back(memory_node);
    memory_node = nullptr;
}

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_MEMORY_POOL_H
