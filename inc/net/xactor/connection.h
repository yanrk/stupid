/********************************************************
 * Description : tcp connection class
 * Data        : 2014-06-30 10:59:25
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_CONNECTION_H
#define STUPID_NET_CONNECTION_H


#include "net/common/common.h"
#include "net/xactor/block.h"
#include "net/xactor/buffer.h"
#include "net/xactor/connection_base.h"
#include "base/utility/uncopy.h"
#include "base/locker/locker.h"
#include "base/pool/object_pool.h"

NAMESPACE_STUPID_NET_BEGIN

#ifdef _MSC_VER
    class TcpProactor;
    typedef TcpProactor   TcpXactor;
#else
    class TcpReactor;
    typedef TcpReactor    TcpXactor;
#endif // _MSC_VER

class TcpConnection : public TcpConnectionBase, private Stupid::Base::Uncopy
{
private:
    typedef Stupid::Base::ObjectPool<Block, Stupid::Base::ThreadLocker> BlockPool;

private:
#ifdef _MSC_VER
    friend class TcpProactor;
#else
    friend class TcpReactor;
#endif // _MSC_VER

public:
    TcpConnection(TcpXactor & xactor, BlockPool & block_pool);
    ~TcpConnection();

public:
    virtual void get_peer_address(std::string & ip, unsigned short & port);

public:
    void recv_buffer_water_mark(size_t length);

public:
    size_t recv_buffer_size();
    bool recv_buffer_copy_len(char * buffer, size_t length);
    bool recv_buffer_move_len(char * buffer, size_t length);
    bool recv_buffer_drop_len(size_t length);

public:
    bool send_buffer_fill_len(const char * data, size_t length);

public:
    void close();

private:
    void set_socket(socket_t sockfd);
    socket_t get_socket();

private:
    void set_listener(socket_t listener);
    socket_t get_listener();

private:
    void set_listener_port(unsigned short listener_port);
    unsigned short get_listener_port();

private:
    void set_unique(size_t unique);
    size_t get_unique();

private:
    void set_connected(bool connected);
    bool get_connected();

private:
    void set_requester(bool requester);
    bool get_requester();

private:
    void set_identity(size_t identity);
    size_t get_identity();

private:
    void set_eof();
    bool get_eof();

private:
    void set_error();
    bool get_error();

private:
    void set_address(const sockaddr_in_t & address);
    sockaddr_in_t get_address();

private:
    void increase_reference();
    void decrease_reference();
    bool has_reference();

private:
    size_t recv_buffer_water_mark();

private:
    bool recv_buffer_fill_len(const char * data, size_t length);

private:
    size_t send_buffer_size();
    bool send_buffer_copy_len(char * buffer, size_t length);
    bool send_buffer_move_len(char * buffer, size_t length);
    bool send_buffer_drop_len(size_t length);

private:
#ifdef _MSC_VER
    struct iocp_event
    {
        OVERLAPPED               overlapped;
        TcpConnection          * connection;
        char                     buffer[1024 * 4];
        size_t                   buffer_size;
        WSABUF                   data;
        size_t                   post_type;
    };

    iocp_event                   m_async_recv;
    iocp_event                   m_async_send;
#endif // _MSC_VER

    TcpXactor                  & m_xactor;
    socket_t                     m_sockfd;
    socket_t                     m_listener;
    unsigned short               m_listener_port;
    size_t                       m_unique;
    bool                         m_connected;
    bool                         m_requester;
    bool                         m_eof;
    bool                         m_error;
    sockaddr_in_t                m_address;
    size_t                       m_identity;
    size_t                       m_references;
    size_t                       m_recv_water_mark;
    Buffer                       m_recv_buffer;
    Buffer                       m_send_buffer;
    Stupid::Base::ThreadLocker   m_locker;
};

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_CONNECTION_H
