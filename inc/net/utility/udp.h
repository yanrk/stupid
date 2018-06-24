/********************************************************
 * Description : simple encapsulations of udp socket
 * Data        : 2013-11-10 21:36:20
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_NET_UDP_H
#define STUPID_NET_UDP_H


#include "net/common/common.h"

NAMESPACE_STUPID_NET_BEGIN

STUPID_CXX_API(bool) udp_listen(const char * host, const char * service, socket_t & listener, bool reuse_address = true, bool broadcast = false);
STUPID_CXX_API(bool) udp_listen(unsigned short port, socket_t & listener, bool reuse_address = true, bool broadcast = false);
STUPID_CXX_API(bool) udp_listen(const sockaddr_in_t & address, socket_t & listener, bool reuse_address = true, bool broadcast = false);
STUPID_CXX_API(bool) udp_connect(const char * host, const char * service, socket_t & connecter, const char * bind_ip = "0.0.0.0", unsigned short bind_port = 0, bool reuse_address = true);
STUPID_CXX_API(bool) udp_connect(const char * ip, unsigned short port, socket_t & connecter, const char * bind_ip = "0.0.0.0", unsigned short bind_port = 0, bool reuse_address = true);
STUPID_CXX_API(bool) udp_connect(const sockaddr_in_t & address, socket_t & connecter, const char * bind_ip = "0.0.0.0", unsigned short bind_port = 0, bool reuse_address = true);
STUPID_CXX_API(bool) udp_accept(socket_t listener, socket_t & accepter, char * buff, size_t buff_size, size_t & recv_size, sockaddr_in_t * address = nullptr, sockaddr_len_t * addr_len = nullptr);
STUPID_CXX_API(bool) udp_accept(socket_t listener, sockaddr_in_t & address, sockaddr_len_t & addr_len, char * buff, size_t buff_size, size_t & recv_size);
STUPID_CXX_API(bool) udp_socket(socket_t & sock);
STUPID_CXX_API(bool) udp_close(socket_t & sock);
STUPID_CXX_API(bool) udp_bind(socket_t sock, const char * bind_ip, unsigned short bind_port);
STUPID_CXX_API(bool) udp_bind(socket_t sock, const sockaddr_in_t & address);
STUPID_CXX_API(bool) udp_set_block_switch(socket_t sock, bool blocking);
STUPID_CXX_API(bool) udp_set_reuse_switch(socket_t sock, bool reuse_address);
STUPID_CXX_API(bool) udp_set_broadcast_switch(socket_t sock, bool broadcast);
STUPID_CXX_API(bool) udp_set_send_timeout(socket_t sock, size_t send_timeout_ms = 30000);
STUPID_CXX_API(bool) udp_set_recv_timeout(socket_t sock, size_t recv_timeout_ms = 30000);
STUPID_CXX_API(bool) udp_set_send_buffer_size(socket_t sock, size_t send_buffsiz = 65535);
STUPID_CXX_API(bool) udp_set_recv_buffer_size(socket_t sock, size_t recv_buffsiz = 65535);
STUPID_CXX_API(bool) udp_send(socket_t sock, const sockaddr_in_t & address, const char * data, size_t data_size);
STUPID_CXX_API(bool) udp_send(socket_t sock, const char * data, size_t data_size);
STUPID_CXX_API(bool) udp_recv(socket_t sock, sockaddr_in_t & address, char * buff, size_t buff_size, size_t & recv_size);
STUPID_CXX_API(bool) udp_recv(socket_t sock, char * buff, size_t buff_size, size_t & recv_size);

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_UDP_H
