/********************************************************
 * Description : net connection data buffer
 * Data        : 2014-06-29 17:23:54
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_BUFFER_H
#define STUPID_NET_BUFFER_H


#include <list>
#include "net/common/common.h"
#include "net/xactor/block.h"
#include "base/utility/uncopy.h"
#include "base/locker/locker.h"
#include "base/pool/object_pool.h"

NAMESPACE_STUPID_NET_BEGIN

class Buffer : private Stupid::Base::Uncopy
{
private:
    typedef Stupid::Base::ObjectPool<Block, Stupid::Base::ThreadLocker> BlockPool;

public:
    Buffer(BlockPool & block_pool);
    ~Buffer();

public:
    size_t size() const;
    bool copy_len(char * buffer, size_t length);
    bool move_len(char * buffer, size_t length);
    bool drop_len(size_t length);
    bool fill_len(const char * data, size_t length);

private:
    Block * acquire_block();
    void release_block(Block *& block);

private:
    BlockPool          & m_block_pool;
    std::list<Block *>   m_blocks;
    size_t               m_length;
};

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_BUFFER_H
