/********************************************************
 * Description : simple encapsulations of tcp socket
 * Data        : 2013-11-10 16:25:42
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER
    #include <ws2tcpip.h>
#else
    #include <unistd.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/socket.h>
#endif // _MSC_VER

#include <cstring>
#include "base/log/log.h"
#include "net/utility/utility.h"
#include "net/utility/tcp.h"

NAMESPACE_STUPID_NET_BEGIN

bool tcp_listen(const char * host, const char * service, socket_t & listener, int backlog)
{
    listener = BAD_SOCKET;

    if (nullptr == host || nullptr == service)
    {
        DBG_LOG("tcp_listen failed: nullptr");
        return(false);
    }

    if (0 == strcmp(host, "127.0.0.1"))
    {
        host = "0.0.0.0";
    }

    struct addrinfo addr_temp;
    memset(&addr_temp, 0x00, sizeof(addr_temp));
    addr_temp.ai_flags = AI_PASSIVE;
    addr_temp.ai_family = AF_UNSPEC;
    addr_temp.ai_socktype = SOCK_STREAM;

    struct addrinfo * addr_info = nullptr;
    if (0 != getaddrinfo(host, service, &addr_temp, &addr_info))
    {
        RUN_LOG_ERR("getaddrinfo failed: %d", stupid_net_error());
        return(false);
    }

    bool ret = false;
    struct addrinfo * addr_dup = addr_info;

    for ( ; nullptr != addr_info; addr_info = addr_info->ai_next)
    {
        socket_t sock = socket(addr_info->ai_family, addr_info->ai_socktype | SOCK_CLOEXEC, addr_info->ai_protocol);
        if (BAD_SOCKET == sock)
        {
            DBG_LOG("socket failed: %d, try again", stupid_net_error());
            continue;
        }

        do
        {
            const int reuse_addr_on = 1;
            const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
            {
                RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
                break;
            }

            if (bind(sock, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)) < 0)
            {
                RUN_LOG_ERR("bind failed: %d", stupid_net_error());
                break;
            }

            if (listen(sock, backlog) < 0)
            {
                RUN_LOG_ERR("listen failed: %d", stupid_net_error());
                break;
            }

            ret = true;
        } while (false);

        if (ret)
        {
            listener = sock;
            break;
        }

        tcp_close(sock);
    }

    freeaddrinfo(addr_dup);

    return(ret);
}

bool tcp_listen(unsigned short port, socket_t & listener, int backlog)
{
    sockaddr_in_t address;
    if (!transform_address("0.0.0.0", port, address))
    {
        RUN_LOG_ERR("transform_address failed");
        listener = BAD_SOCKET;
        return(false);
    }
    return(tcp_listen(address, listener, backlog));
}

bool tcp_listen(const sockaddr_in_t & address, socket_t & listener, int backlog)
{
    listener = BAD_SOCKET;

    socket_t sock = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (BAD_SOCKET == sock)
    {
        RUN_LOG_ERR("socket failed: %d", stupid_net_error());
        return(false);
    }

    do
    {
        const int reuse_addr_on = 1;
        const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
        {
            RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
            break;
        }

        if (bind(sock, reinterpret_cast<sockaddr_t *>(const_cast<sockaddr_in_t *>(&address)), sizeof(address)) < 0)
        {
            RUN_LOG_ERR("bind failed: %d", stupid_net_error());
            break;
        }

        if (listen(sock, backlog) < 0)
        {
            RUN_LOG_ERR("listen failed: %d", stupid_net_error());
            break;
        }

        listener = sock;

        return(true);
    } while (false);

    tcp_close(sock);

    return(false);
}

bool tcp_connect(const char * host, const char * service, socket_t & connecter, const char * bind_ip, unsigned short bind_port)
{
    connecter = BAD_SOCKET;

    if (nullptr == host || nullptr == service)
    {
        DBG_LOG("tcp_connect failed: nullptr");
        return(false);
    }

    sockaddr_in_t bind_address;
    if (0 != bind_port)
    {
        if (nullptr == bind_ip || 0 == strcmp(bind_ip, "127.0.0.1"))
        {
            bind_ip = "0.0.0.0";
        }
        if (!transform_address(bind_ip, bind_port, bind_address))
        {
            RUN_LOG_ERR("transform_address failed");
            return(false);
        }
    }

    struct addrinfo addr_temp;
    memset(&addr_temp, 0x00, sizeof(addr_temp));
    addr_temp.ai_family = AF_UNSPEC;
    addr_temp.ai_socktype = SOCK_STREAM;

    struct addrinfo * addr_info = nullptr;
    if (0 != getaddrinfo(host, service, &addr_temp, &addr_info))
    {
        RUN_LOG_ERR("getaddrinfo failed: %d", stupid_net_error());
        return(false);
    }

    bool ret = false;
    struct addrinfo * addr_dup = addr_info;

    for ( ; nullptr != addr_info; addr_info = addr_info->ai_next)
    {
        socket_t sock = socket(addr_info->ai_family, addr_info->ai_socktype | SOCK_CLOEXEC, addr_info->ai_protocol);
        if (BAD_SOCKET == sock)
        {
            DBG_LOG("socket failed: %d, try again", stupid_net_error());
            continue;
        }

        if (0 != bind_port)
        {
            const int reuse_addr_on = 1;
            const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
            {
                RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
                tcp_close(sock);
                continue;
            }

            if (bind(sock, reinterpret_cast<sockaddr_t *>(&bind_address), sizeof(bind_address)) < 0)
            {
                RUN_LOG_ERR("bind failed: %d, try again", stupid_net_error());
                tcp_close(sock);
                continue;
            }
        }

        if (connect(sock, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)) < 0)
        {
            RUN_LOG_ERR("connect failed: %d", stupid_net_error());
            tcp_close(sock);
        }
        else
        {
            connecter = sock;
            ret = true;
            break;
        }
    }

    freeaddrinfo(addr_dup);

    return(ret);
}

bool tcp_connect(const char * ip, unsigned short port, socket_t & connecter, const char * bind_ip, unsigned short bind_port)
{
    sockaddr_in_t address;
    if (!transform_address(ip, port, address))
    {
        RUN_LOG_ERR("transform_address failed");
        connecter = BAD_SOCKET;
        return(false);
    }
    return(tcp_connect(address, connecter, bind_ip, bind_port));
}

bool tcp_connect(const sockaddr_in_t & address, socket_t & connecter, const char * bind_ip, unsigned short bind_port)
{
    connecter = BAD_SOCKET;

    socket_t sock = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (BAD_SOCKET == sock)
    {
        RUN_LOG_ERR("socket failed: %d", stupid_net_error());
        return(false);
    }

    if (0 != bind_port)
    {
        sockaddr_in_t bind_address;
        if (nullptr == bind_ip || 0 == strcmp(bind_ip, "127.0.0.1"))
        {
            bind_ip = "0.0.0.0";
        }
        if (!transform_address(bind_ip, bind_port, bind_address))
        {
            RUN_LOG_ERR("transform_address failed");
            tcp_close(sock);
            return(false);
        }

        const int reuse_addr_on = 1;
        const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
        {
            RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
            tcp_close(sock);
            return(false);
        }

        if (bind(sock, reinterpret_cast<sockaddr_t *>(&bind_address), sizeof(bind_address)) < 0)
        {
            RUN_LOG_ERR("bind failed: %d", stupid_net_error());
            tcp_close(sock);
            return(false);
        }
    }

    if (connect(sock, reinterpret_cast<sockaddr_t *>(const_cast<sockaddr_in_t *>(&address)), sizeof(address)) < 0)
    {
        RUN_LOG_ERR("connect failed: %d", stupid_net_error());
        tcp_close(sock);
        return(false);
    }

    connecter = sock;

    return(true);
}

bool tcp_accept(socket_t listener, socket_t & accepter, sockaddr_in_t * address, sockaddr_len_t * addr_len)
{
    accepter = BAD_SOCKET;

    sockaddr_in_t remote_address;
    memset(&remote_address, 0x00, sizeof(remote_address));
    sockaddr_len_t remote_addr_len = sizeof(remote_address);

    socket_t sock = accept(listener, reinterpret_cast<sockaddr_t *>(&remote_address), &remote_addr_len);
    if (BAD_SOCKET == sock)
    {
        if (!stupid_is_net_blocking_error())
        {
            RUN_LOG_ERR("tcp_accept failed: %d", stupid_net_error());
        }
        return(false);
    }

    if (nullptr != address)
    {
        memcpy(address, &remote_address, sizeof(remote_address));
    }

    if (nullptr != addr_len)
    {
        memcpy(addr_len, &remote_addr_len, sizeof(remote_addr_len));
    }

    accepter = sock;

#ifndef _MSC_VER
    int flags = fcntl(sock, F_GETFD, 0);
    if (flags < 0)
    {
        RUN_LOG_WAR("fcntl(get-%s-flags) failed: %d", "cloexec", stupid_net_error());
        return(true);
    }
    flags |= FD_CLOEXEC;
    int ret = fcntl(sock, F_SETFD, flags);
    if (ret < 0)
    {
        RUN_LOG_WAR("fcntl(set-%s-flags) failed: %d", "cloexec", stupid_net_error());
        return(true);
    }
#endif // _MSC_VER

    return(true);
}

bool tcp_socket(socket_t & sock)
{
    sock = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (BAD_SOCKET == sock)
    {
        RUN_LOG_ERR("socket failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool tcp_close(socket_t & sock)
{
    if (BAD_SOCKET != sock)
    {
        if (stupid_close_socket(sock) < 0)
        {
            if (!stupid_is_net_blocking_error())
            {
                RUN_LOG_ERR("close failed: %d", stupid_net_error());
            }
            return(false);
        }
        sock = BAD_SOCKET;
    }
    return(true);
}

bool tcp_set_block_switch(socket_t sock, bool blocking)
{
#ifdef _MSC_VER
    u_long non_blocking_mode = (blocking ? 0 : 1);
    int ret = ioctlsocket(sock, FIONBIO, &non_blocking_mode);
    if (ret < 0)
    {
        RUN_LOG_ERR("ioctlsocket(%s) failed: %d", (blocking ? "blocking" : "non-blocking"), stupid_net_error());
        return(false);
    }
    return(true);
#else
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0)
    {
        RUN_LOG_ERR("fcntl(get-%s-flags) failed: %d", (blocking ? "blocking" : "non-blocking"), stupid_net_error());
        return(false);
    }
    if (blocking)
    {
        flags &= ~O_NONBLOCK;
    }
    else
    {
        flags |= O_NONBLOCK;
    }
    int ret = fcntl(sock, F_SETFL, flags);
    if (ret < 0)
    {
        RUN_LOG_ERR("fcntl(set-%s-flags) failed: %d", (blocking ? "blocking" : "non-blocking"), stupid_net_error());
        return(false);
    }
    return(true);
#endif // _MSC_VER
}

bool tcp_set_reuse_switch(socket_t sock, bool reuse_address)
{
    int reuse_addr_on = (reuse_address ? 1 : 0);
    int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&reuse_addr_on), sizeof(reuse_addr_on));
    if (ret < 0)
    {
        RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool tcp_set_linger_switch(socket_t sock, bool forced_to_close, size_t wait_seconds)
{
    struct linger linger_switch;
    linger_switch.l_onoff = (forced_to_close ? 1 : 0);
#ifdef _MSC_VER
    linger_switch.l_linger = static_cast<u_short>(wait_seconds);
#else
    linger_switch.l_linger = static_cast<int>(wait_seconds);
#endif // _MSC_VER
    int ret = setsockopt(sock, SOL_SOCKET, SO_LINGER, reinterpret_cast<char *>(&linger_switch), sizeof(linger_switch));
    if (ret < 0)
    {
        RUN_LOG_ERR("setsockopt(linger) failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool tcp_set_send_timeout(socket_t sock, size_t send_timeout_ms)
{
#ifdef _MSC_VER
    int timeout = static_cast<int>(send_timeout_ms);
#else
    struct timeval timeout = { static_cast<time_t>(send_timeout_ms / 1000), static_cast<suseconds_t>(send_timeout_ms % 1000 * 1000) };
#endif // _MSC_VER
    int ret = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char *>(&timeout), sizeof(timeout));
    if (ret < 0)
    {
        RUN_LOG_ERR("setsockopt(send-timeout) failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool tcp_set_recv_timeout(socket_t sock, size_t recv_timeout_ms)
{
#ifdef _MSC_VER
    int timeout = static_cast<int>(recv_timeout_ms);
#else
    struct timeval timeout = { static_cast<time_t>(recv_timeout_ms / 1000), static_cast<suseconds_t>(recv_timeout_ms % 1000 * 1000) };
#endif // _MSC_VER
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&timeout), sizeof(timeout));
    if (ret < 0)
    {
        RUN_LOG_ERR("setsockopt(recv-timeout) failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool tcp_set_send_buffer_size(socket_t sock, size_t send_buffsiz)
{
    int buffsize = static_cast<int>(send_buffsiz);
    int ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&buffsize), sizeof(buffsize));
    if (ret < 0)
    {
        RUN_LOG_ERR("setsockopt(send-buf) failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool tcp_set_recv_buffer_size(socket_t sock, size_t recv_buffsiz)
{
    int buffsize = static_cast<int>(recv_buffsiz);
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&buffsize), sizeof(buffsize));
    if (ret < 0)
    {
        RUN_LOG_ERR("setsockopt(recv-buf) failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool tcp_send_complete(socket_t sock, const char * data, size_t data_len)
{
    if (nullptr == data)
    {
        DBG_LOG("tcp_send_complete failed: nullptr");
        return(false);
    }

    while (data_len > 0)
    {
        int send_len = stupid_send(sock, data, static_cast<int>(data_len));
        if (send_len < 0)
        {
            if (stupid_is_net_blocking_error())
            {
                continue;
            }
            else
            {
                RUN_LOG_TRK("tcp_send_complete failed: %d", stupid_net_error()); /* DBG_LOG */
                return(false);
            }
        }
        else
        {
            data += send_len;
            data_len -= send_len;
        }
    }
    return(true);
}

bool tcp_recv_complete(socket_t sock, char * buff, size_t need_len)
{
    if (nullptr == buff)
    {
        DBG_LOG("tcp_recv_complete failed: nullptr");
        return(false);
    }

    while (need_len > 0)
    {
        int recv_len = stupid_recv(sock, buff, static_cast<int>(need_len));
        if (recv_len < 0)
        {
            if (stupid_is_net_blocking_error())
            {
                continue;
            }
            else
            {
                RUN_LOG_TRK("tcp_recv_complete failed: %d", stupid_net_error()); /* DBG_LOG */
                return(false);
            }
        }
        else if (0 == recv_len)
        {
            RUN_LOG_TRK("tcp_recv_complete failed: remote close"); /* DBG_LOG */
            return(false);
        }
        else
        {
            buff += recv_len;
            need_len -= recv_len;
        }
    }
    return(true);
}

NAMESPACE_STUPID_NET_END
