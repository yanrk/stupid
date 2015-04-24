/********************************************************
 * Description : net connection data block
 * Data        : 2014-06-29 13:09:15
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_BLOCK_H
#define STUPID_NET_BLOCK_H


#include "net/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_NET_BEGIN

class Block : private Stupid::Base::Uncopy
{
public:
    Block();
    ~Block();

public:
    void reset();
    const char * data() const;
    size_t size() const;
    size_t capacity() const;
    bool append(const char * data, size_t length);
    bool offset(size_t length);

private:
    char           m_buffer[1024 * 8];
    char         * m_data;
    size_t         m_size;
    size_t         m_capacity;
};

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_BLOCK_H
