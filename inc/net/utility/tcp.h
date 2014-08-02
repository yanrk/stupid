/********************************************************
 * Description : simple encapsulations of tcp socket
 * Data        : 2013-11-10 16:25:42
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_TCP_H
#define STUPID_NET_TCP_H


#include "net/common/common.h"

NAMESPACE_STUPID_NET_BEGIN

STUPID_CXX_API(bool) tcp_listen(const char * host, const char * service, socket_t & listener, int backlog = 5);
STUPID_CXX_API(bool) tcp_listen(unsigned short port, socket_t & listener, int backlog = 5);
STUPID_CXX_API(bool) tcp_listen(const sockaddr_in_t & address, socket_t & listener, int backlog = 5);
STUPID_CXX_API(bool) tcp_connect(const char * host, const char * service, socket_t & connecter);
STUPID_CXX_API(bool) tcp_connect(const char * ip, unsigned short port, socket_t & connecter);
STUPID_CXX_API(bool) tcp_connect(const sockaddr_in_t & address, socket_t & connecter);
STUPID_CXX_API(bool) tcp_accept(socket_t listener, socket_t & accepter, sockaddr_in_t * address = nullptr, sock_len_t * addr_len = nullptr);
STUPID_CXX_API(void) tcp_close(socket_t & sock);
STUPID_CXX_API(bool) tcp_set_block_switch(socket_t sock, bool blocking);
STUPID_CXX_API(bool) tcp_set_send_timeout(socket_t sock, size_t send_timeout_ms = 30000);
STUPID_CXX_API(bool) tcp_set_recv_timeout(socket_t sock, size_t recv_timeout_ms = 30000);
STUPID_CXX_API(bool) tcp_set_send_buffer_size(socket_t sock, size_t send_buffsiz = 65535);
STUPID_CXX_API(bool) tcp_set_recv_buffer_size(socket_t sock, size_t recv_buffsiz = 65535);
STUPID_CXX_API(bool) tcp_send_complete(socket_t sock, const char * data, size_t data_len);
STUPID_CXX_API(bool) tcp_recv_complete(socket_t sock, char * buff, size_t need_len);

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_TCP_H
