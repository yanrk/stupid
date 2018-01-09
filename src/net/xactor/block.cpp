/********************************************************
 * Description : net connection data block
 * Data        : 2014-06-29 13:09:15
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cstring>
#include "net/xactor/block.h"

NAMESPACE_STUPID_NET_BEGIN

Block::Block()
{
    reset();
}

Block::~Block()
{

}

void Block::reset()
{
    m_data = m_buffer;
    m_size = 0;
    m_capacity = sizeof(m_buffer) / sizeof(m_buffer[0]);
}

const char * Block::data() const
{
    return(m_data);
}

size_t Block::size() const
{
    return(m_size);
}

size_t Block::capacity() const
{
    return(m_capacity);
}

bool Block::append(const char * data, size_t length)
{
    if (nullptr == data || m_capacity < length)
    {
        return(false);
    }

    memcpy(m_data + m_size, data, length);
    m_size += length;
    m_capacity -= length;

    return(true);
}

bool Block::offset(size_t length)
{
    if (m_size < length)
    {
        return(false);
    }

    m_data += length;
    m_size -= length;

    return(true);
}

NAMESPACE_STUPID_NET_END
