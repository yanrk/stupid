/********************************************************
 * Description : net connection data buffer
 * Data        : 2014-06-29 17:23:54
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <cstring>
#include <algorithm>
#include "net/xactor/buffer.h"

NAMESPACE_STUPID_NET_BEGIN

Buffer::Buffer(BlockPool & block_pool)
    : m_block_pool(block_pool)
    , m_blocks()
    , m_length(0)
{

}

Buffer::~Buffer()
{
    m_block_pool << m_blocks;
}

Block * Buffer::acquire_block()
{
    Block * block = m_block_pool.acquire();
    if (nullptr != block)
    {
        block->reset();
    }
    return(block);
}

void Buffer::release_block(Block *& block)
{
    m_block_pool.release(block);
}

bool Buffer::fill_len(const char * data, size_t length)
{
    if (nullptr == data)
    {
        return(false);
    }

    if (0 == length)
    {
        return(true);
    }

    Block * block = nullptr;
    if (!m_blocks.empty())
    {
        block = m_blocks.back();
    }

    while (0 != length)
    {
        if (nullptr == block || 0 == block->capacity())
        {
            block = acquire_block();
            if (nullptr == block)
            {
                return(false);
            }
            m_blocks.push_back(block);
        }
        size_t data_len = std::min<size_t>(length, block->capacity());
        block->append(data, data_len);
        data += data_len;
        length -= data_len;
        m_length += data_len;
    }

    return(true);
}

size_t Buffer::size() const
{
    return(m_length);
}

bool Buffer::copy_len(char * buffer, size_t length)
{
    if (nullptr == buffer || m_length < length)
    {
        return(false);
    }

    typedef std::list<Block *>::iterator iterator;
    for (iterator iter = m_blocks.begin(); m_blocks.end() != iter && 0 != length; ++iter)
    {
        Block * block = *iter;
        size_t data_len = std::min<size_t>(length, block->size());
        memcpy(buffer, block->data(), data_len);
        buffer += data_len;
        length -= data_len;
    }

    return(true);
}

bool Buffer::move_len(char * buffer, size_t length)
{
    return(copy_len(buffer, length) && drop_len(length));
}

bool Buffer::drop_len(size_t length)
{
    if (m_length < length)
    {
        return(false);
    }

    if (0 == length)
    {
        return(true);
    }

    m_length -= length;

    while (true)
    {
        Block * block = m_blocks.front();
        if (block->size() < length)
        {
            length -= block->size();
            m_blocks.pop_front();
            release_block(block);
        }
        else
        {
            block->offset(length);
            break;
        }
    }

    return(true);
}

NAMESPACE_STUPID_NET_END
