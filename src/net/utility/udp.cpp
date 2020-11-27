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
#include "net/utility/udp.h"

NAMESPACE_STUPID_NET_BEGIN

bool udp_listen(const char * host, const char * service, socket_t & listener, bool reuse_address, bool broadcast)
{
    listener = BAD_SOCKET;

    if (nullptr == host || nullptr == service)
    {
        DBG_LOG("udp_listen failed: nullptr");
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
    addr_temp.ai_socktype = SOCK_DGRAM;

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
        socket_t sock = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
        if (BAD_SOCKET == sock)
        {
            DBG_LOG("socket failed: %d, try again", stupid_net_error());
            continue;
        }

        do
        {
            if (reuse_address)
            {
                const int reuse_addr_on = 1;
                const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
                if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
                {
                    RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
                    break;
                }
            }

            if (broadcast)
            {
                const int broadcast_on = 1;
                const char * broadcast_ptr = reinterpret_cast<const char *>(&broadcast_on);
                if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, broadcast_ptr, sizeof(broadcast_on)) < 0)
                {
                    RUN_LOG_ERR("setsockopt(broadcast) failed: %d", stupid_net_error());
                    break;
                }
            }

            if (bind(sock, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)) < 0)
            {
                RUN_LOG_ERR("bind failed: %d", stupid_net_error());
                break;
            }

            ret = true;
        } while (false);

        if (ret)
        {
            listener = sock;
            break;
        }

        udp_close(sock);
    }

    freeaddrinfo(addr_dup);

    return(ret);
}

bool udp_listen(unsigned short port, socket_t & listener, bool reuse_address, bool broadcast)
{
    sockaddr_in_t address;
    if (!transform_address("0.0.0.0", port, address))
    {
        RUN_LOG_ERR("transform_address failed");
        listener = BAD_SOCKET;
        return(false);
    }
    return(udp_listen(address, listener, reuse_address, broadcast));
}

bool udp_listen(const sockaddr_in_t & address, socket_t & listener, bool reuse_address, bool broadcast)
{
    listener = BAD_SOCKET;

    socket_t sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (BAD_SOCKET == sock)
    {
        RUN_LOG_ERR("socket failed: %d", stupid_net_error());
        return(false);
    }

    do
    {
        if (reuse_address)
        {
            const int reuse_addr_on = 1;
            const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
            {
                RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
                break;
            }
        }

        if (broadcast)
        {
            const int broadcast_on = 1;
            const char * broadcast_ptr = reinterpret_cast<const char *>(&broadcast_on);
            if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, broadcast_ptr, sizeof(broadcast_on)) < 0)
            {
                RUN_LOG_ERR("setsockopt(broadcast) failed: %d", stupid_net_error());
                break;
            }
        }

        if (bind(sock, reinterpret_cast<sockaddr_t *>(const_cast<sockaddr_in_t *>(&address)), sizeof(address)) < 0)
        {
            RUN_LOG_ERR("bind failed: %d", stupid_net_error());
            break;
        }

        listener = sock;

        return(true);
    } while (false);

    udp_close(sock);

    return(false);
}

bool udp_connect(const char * host, const char * service, socket_t & connecter, const char * bind_ip, unsigned short bind_port, bool reuse_address)
{
    connecter = BAD_SOCKET;

    if (nullptr == host || nullptr == service)
    {
        DBG_LOG("udp_connect failed: nullptr");
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
    addr_temp.ai_socktype = SOCK_DGRAM;

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
        socket_t sock = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
        if (BAD_SOCKET == sock)
        {
            DBG_LOG("socket failed: %d, try again", stupid_net_error());
            continue;
        }

        if (0 != bind_port)
        {
            if (reuse_address)
            {
                const int reuse_addr_on = 1;
                const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
                if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
                {
                    RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
                    udp_close(sock);
                    continue;
                }
            }

            if (bind(sock, reinterpret_cast<sockaddr_t *>(&bind_address), sizeof(bind_address)) < 0)
            {
                RUN_LOG_ERR("bind failed: %d, try again", stupid_net_error());
                udp_close(sock);
                continue;
            }
        }

        if (connect(sock, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)) < 0)
        {
            RUN_LOG_ERR("connect failed: %d", stupid_net_error());
            udp_close(sock);
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

bool udp_connect(const char * ip, unsigned short port, socket_t & connecter, const char * bind_ip, unsigned short bind_port, bool reuse_address)
{
    sockaddr_in_t address;
    if (!transform_address(ip, port, address))
    {
        RUN_LOG_ERR("transform_address failed");
        connecter = BAD_SOCKET;
        return(false);
    }
    return(udp_connect(address, connecter, bind_ip, bind_port, reuse_address));
}

bool udp_connect(const sockaddr_in_t & address, socket_t & connecter, const char * bind_ip, unsigned short bind_port, bool reuse_address)
{
    connecter = BAD_SOCKET;

    socket_t sock = socket(AF_INET, SOCK_DGRAM, 0);
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
            udp_close(sock);
            return(false);
        }

        if (reuse_address)
        {
            const int reuse_addr_on = 1;
            const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
            {
                RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
                udp_close(sock);
                return(false);
            }
        }

        if (bind(sock, reinterpret_cast<sockaddr_t *>(&bind_address), sizeof(bind_address)) < 0)
        {
            RUN_LOG_ERR("bind failed: %d", stupid_net_error());
            udp_close(sock);
            return(false);
        }
    }

    if (connect(sock, reinterpret_cast<sockaddr_t *>(const_cast<sockaddr_in_t *>(&address)), sizeof(address)) < 0)
    {
        RUN_LOG_ERR("connect failed: %d", stupid_net_error());
        udp_close(sock);
        return(false);
    }

    connecter = sock;

    return(true);
}

bool udp_accept(socket_t listener, socket_t & accepter, char * buff, size_t buff_size, size_t & recv_size, sockaddr_in_t * address, sockaddr_len_t * addr_len)
{
    accepter = BAD_SOCKET;

    if (nullptr == buff)
    {
        DBG_LOG("buffer is nullptr");
        return(false);
    }

    sockaddr_in_t remote_address;
    memset(&remote_address, 0x00, sizeof(remote_address));
    sockaddr_len_t remote_addr_len = sizeof(remote_address);

    int recv_len = recvfrom(listener, buff, static_cast<int>(buff_size), 0, reinterpret_cast<sockaddr_t *>(&remote_address), &remote_addr_len);
    if (recv_len < 0)
    {
        if (!stupid_is_net_blocking_error())
        {
            RUN_LOG_ERR("udp_accept failed: %d", stupid_net_error());
        }
        return(false);
    }
    recv_size = static_cast<size_t>(recv_len);

    socket_t sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (BAD_SOCKET == sock)
    {
        RUN_LOG_ERR("socket failed: %d", stupid_net_error());
        return(false);
    }

    if (connect(sock, reinterpret_cast<sockaddr_t *>(&remote_address), remote_addr_len) < 0)
    {
        RUN_LOG_ERR("connect failed: %d", stupid_net_error());
        udp_close(sock);
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

    return(true);
}

bool udp_accept(socket_t listener, sockaddr_in_t & address, sockaddr_len_t & addr_len, char * buff, size_t buff_size, size_t & recv_size)
{
    if (nullptr == buff)
    {
        DBG_LOG("buffer is nullptr");
        return(false);
    }

    memset(&address, 0x00, sizeof(address));
    addr_len = sizeof(address);

    int recv_len = recvfrom(listener, buff, static_cast<int>(buff_size), 0, reinterpret_cast<sockaddr_t *>(&address), &addr_len);
    if (recv_len < 0)
    {
        if (!stupid_is_net_blocking_error())
        {
            RUN_LOG_ERR("udp_accept failed: %d", stupid_net_error());
        }
        return(false);
    }
    recv_size = static_cast<size_t>(recv_len);

    return(true);
}

bool udp_socket(socket_t & sock)
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (BAD_SOCKET == sock)
    {
        RUN_LOG_ERR("socket failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool udp_close(socket_t & sock)
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

bool udp_bind(socket_t sock, const char * bind_ip, unsigned short bind_port)
{
    sockaddr_in_t address;
    if (!transform_address(bind_ip, bind_port, address))
    {
        RUN_LOG_ERR("transform_address failed");
        return(false);
    }
    return(udp_bind(sock, address));
}

bool udp_bind(socket_t sock, const sockaddr_in_t & address)
{
    if (bind(sock, reinterpret_cast<const sockaddr_t *>(&address), sizeof(address)) < 0)
    {
        RUN_LOG_ERR("bind failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool udp_set_block_switch(socket_t sock, bool blocking)
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

bool udp_set_reuse_switch(socket_t sock, bool reuse_address)
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

bool udp_set_broadcast_switch(socket_t sock, bool broadcast)
{
    int broadcast_on = (broadcast ? 1 : 0);
    int ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&broadcast_on), sizeof(broadcast_on));
    if (ret < 0)
    {
        RUN_LOG_ERR("setsockopt(broadcast) failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool udp_set_send_timeout(socket_t sock, size_t send_timeout_ms)
{
#ifdef _MSC_VER
    int timeout = static_cast<int>(send_timeout_ms);
#else
    struct timeval timeout = { static_cast<time_t>(send_timeout_ms) / 1000, static_cast<suseconds_t>(send_timeout_ms % 1000 * 1000) };
#endif // _MSC_VER
    int ret = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char *>(&timeout), sizeof(timeout));
    if (ret < 0)
    {
        RUN_LOG_ERR("setsockopt(send-timeout) failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

bool udp_set_recv_timeout(socket_t sock, size_t recv_timeout_ms)
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

bool udp_set_send_buffer_size(socket_t sock, size_t send_buffsiz)
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

bool udp_set_recv_buffer_size(socket_t sock, size_t recv_buffsiz)
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

bool udp_send(socket_t sock, const sockaddr_in_t & address, const char * data, size_t data_size)
{
    if (nullptr == data)
    {
        DBG_LOG("udp_send failed: nullptr");
        return(false);
    }

    sockaddr_len_t addr_len = sizeof(address);
    int send_len = sendto(sock, data, static_cast<int>(data_size), 0, reinterpret_cast<const sockaddr_t *>(&address), addr_len);
    if (send_len < 0 || send_len != static_cast<int>(data_size))
    {
        if (!stupid_is_net_blocking_error())
        {
            RUN_LOG_TRK("udp_send failed: %d", stupid_net_error()); /* DBG_LOG */
        }
        return(false);
    }

    return(true);
}

bool udp_send(socket_t sock, const char * data, size_t data_size)
{
    if (nullptr == data)
    {
        DBG_LOG("udp_send failed: nullptr");
        return(false);
    }

    int send_len = stupid_send(sock, data, static_cast<int>(data_size));
    if (send_len < 0 || send_len != static_cast<int>(data_size))
    {
        if (!stupid_is_net_blocking_error())
        {
            RUN_LOG_TRK("udp_send failed: %d", stupid_net_error()); /* DBG_LOG */
        }
        return(false);
    }

    return(true);
}

bool udp_recv(socket_t sock, sockaddr_in_t & address, char * buff, size_t buff_size, size_t & recv_size)
{
    if (nullptr == buff)
    {
        DBG_LOG("udp_recv failed: nullptr");
        return(false);
    }

    sockaddr_len_t addr_len = sizeof(address);
    int recv_len = recvfrom(sock, buff, static_cast<int>(buff_size), 0, reinterpret_cast<sockaddr_t *>(&address), &addr_len);
    if (recv_len < 0)
    {
        if (!stupid_is_net_blocking_error())
        {
            RUN_LOG_TRK("udp_recv failed: %d", stupid_net_error()); /* DBG_LOG */
        }
        return(false);
    }
    recv_size = static_cast<size_t>(recv_len);

    return(true);
}

bool udp_recv(socket_t sock, char * buff, size_t buff_size, size_t & recv_size)
{
    if (nullptr == buff)
    {
        DBG_LOG("udp_recv failed: nullptr");
        return(false);
    }

    int recv_len = stupid_recv(sock, buff, static_cast<int>(buff_size));
    if (recv_len < 0)
    {
        if (!stupid_is_net_blocking_error())
        {
            RUN_LOG_TRK("udp_recv failed: %d", stupid_net_error()); /* DBG_LOG */
        }
        return(false);
    }
    recv_size = static_cast<size_t>(recv_len);

    return(true);
}

NAMESPACE_STUPID_NET_END
