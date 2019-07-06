/********************************************************
 * Description : tcp connection class
 * Data        : 2014-06-30 10:59:25
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef _MSC_VER
	#include <arpa/inet.h>
#endif // _MSC_VER

#include <cstring>
#include <cstdlib>
#include "net/xactor/connection.h"
#include "net/utility/tcp.h"
#include "base/utility/guard.h"
#include "net/xactor/proactor.h"
#include "net/xactor/reactor.h"

NAMESPACE_STUPID_NET_BEGIN

TcpConnection::TcpConnection(TcpXactor & xactor, BlockPool & block_pool)
    : TcpConnectionBase()
    , m_xactor(xactor)
    , m_sockfd(BAD_SOCKET)
    , m_listener(BAD_SOCKET)
    , m_listener_port(0)
    , m_unique(0)
    , m_connected(false)
    , m_requester(false)
    , m_eof(false)
    , m_error(false)
    , m_address()
    , m_identity(~static_cast<size_t>(0))
    , m_references(0)
    , m_recv_water_mark(1)
    , m_recv_buffer(block_pool)
    , m_send_buffer(block_pool)
    , m_locker()
{
#ifdef _MSC_VER
    memset(&m_async_recv.overlapped, 0x00, sizeof(m_async_recv.overlapped));
    m_async_recv.connection = this;
    m_async_recv.post_type = 0;
    m_async_recv.buffer_size = sizeof(m_async_recv.buffer) / sizeof(m_async_recv.buffer[0]);
    m_async_recv.data.buf = m_async_recv.buffer;
    m_async_recv.data.len = static_cast<ULONG>(m_async_recv.buffer_size);

    memset(&m_async_send.overlapped, 0x00, sizeof(m_async_send.overlapped));
    m_async_send.connection = this;
    m_async_send.post_type = 0;
    m_async_send.buffer_size = sizeof(m_async_send.buffer) / sizeof(m_async_send.buffer[0]);
    m_async_send.data.buf = m_async_send.buffer;
    m_async_send.data.len = 0;
#endif // _MSC_VER
}

TcpConnection::~TcpConnection()
{
    tcp_close(m_sockfd);
}

void TcpConnection::get_peer_address(std::string & ip, unsigned short & port)
{
    ip = inet_ntoa(m_address.sin_addr);
    port = ntohs(m_address.sin_port);
}

void TcpConnection::set_socket(socket_t sockfd)
{
    m_sockfd = sockfd;
}

socket_t TcpConnection::get_socket()
{
    return(m_sockfd);
}

void TcpConnection::set_listener(socket_t listener)
{
    m_listener = listener;
}

socket_t TcpConnection::get_listener()
{
    return(m_listener);
}

void TcpConnection::set_listener_port(unsigned short listener_port)
{
    m_listener_port = listener_port;
}

unsigned short TcpConnection::get_listener_port()
{
    return(m_listener_port);
}

void TcpConnection::set_unique(size_t unique)
{
    m_unique = unique;
}

size_t TcpConnection::get_unique()
{
    return(m_unique);
}

void TcpConnection::set_connected(bool connected)
{
    m_connected = connected;
}

bool TcpConnection::get_connected()
{
    return(m_connected);
}

void TcpConnection::set_requester(bool requester)
{
    m_requester = requester;
}

bool TcpConnection::get_requester()
{
    return(m_requester);
}

void TcpConnection::set_identity(size_t identity)
{
    m_identity = identity;
}

size_t TcpConnection::get_identity()
{
    return(m_identity);
}

void TcpConnection::set_eof()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    m_eof = true; /* no more recv, but the send-buffer will be send */
}

bool TcpConnection::get_eof()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    return(m_eof);
}

void TcpConnection::set_error()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    m_error = true; /* no more send, no more recv */
}

bool TcpConnection::get_error()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    return(m_error);
}

void TcpConnection::close()
{
    {
        Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
        if (m_eof || m_error)
        {
            return;
        }

        m_eof = true; /* no more recv */
        if (0 != m_send_buffer.size()) /* if the send-buffer is empty */
        {
            return;
        }
    }

    m_xactor.close_connection(this); /* no more recv and no more send */
}

void TcpConnection::set_address(const sockaddr_in_t & address)
{
    memcpy(&m_address, &address, sizeof(m_address));
}

sockaddr_in_t TcpConnection::get_address()
{
    return(m_address);
}

void TcpConnection::increase_reference()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    m_references += 1;
}

void TcpConnection::decrease_reference()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    m_references -= 1;
}

bool TcpConnection::has_reference()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    return(0 != m_references);
}

void TcpConnection::recv_buffer_water_mark(size_t length)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    m_recv_water_mark = length;
}

size_t TcpConnection::recv_buffer_water_mark()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    return(m_recv_water_mark);
}

size_t TcpConnection::recv_buffer_size()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    return(m_recv_buffer.size());
}

bool TcpConnection::recv_buffer_fill_len(const char * data, size_t length)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_eof || m_error)
    {
        return(false);
    }
    return(m_recv_buffer.fill_len(data, length));
}

bool TcpConnection::recv_buffer_copy_len(char * buffer, size_t length)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_eof)
    {
        return(false);
    }
    return(m_recv_buffer.copy_len(buffer, length));
}

bool TcpConnection::recv_buffer_move_len(char * buffer, size_t length)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_eof)
    {
        return(false);
    }
    return(m_recv_buffer.move_len(buffer, length));
}

bool TcpConnection::recv_buffer_drop_len(size_t length)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_eof)
    {
        return(false);
    }
    return(m_recv_buffer.drop_len(length));
}

size_t TcpConnection::send_buffer_size()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    return(m_send_buffer.size());
}

bool TcpConnection::send_buffer_fill_len(const char * data, size_t length)
{
    size_t old_send_buffer_size = 0;
    size_t new_send_buffer_size = 0;

    {
        Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
        if (m_eof || m_error)
        {
            return(false);
        }
        old_send_buffer_size = m_send_buffer.size();
        if (!m_send_buffer.fill_len(data, length))
        {
            return(false);
        }
        new_send_buffer_size = m_send_buffer.size();
    }

    if (0 == old_send_buffer_size && 0 != new_send_buffer_size)
    {
        m_xactor.connection_send(this);
    }

    return(true);
}

bool TcpConnection::send_buffer_copy_len(char * buffer, size_t length)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_error)
    {
        return(false);
    }
    return(m_send_buffer.copy_len(buffer, length));
}

bool TcpConnection::send_buffer_move_len(char * buffer, size_t length)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_error)
    {
        return(false);
    }
    return(m_send_buffer.move_len(buffer, length));
}

bool TcpConnection::send_buffer_drop_len(size_t length)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_error)
    {
        return(false);
    }
    return(m_send_buffer.drop_len(length));
}

NAMESPACE_STUPID_NET_END
