/********************************************************
 * Description : simple encapsulations of udp socket
 * Data        : 2013-11-10 21:36:20
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifdef _MSC_VER
    #include <ws2tcpip.h>
#else
    #include <netdb.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/socket.h>
#endif // _MSC_VER

#include <cstring>
#include "base/log/log.h"
#include "net/utility/udp.h"

NAMESPACE_STUPID_NET_BEGIN

bool udp_bind(const char * host, const char * service, socket_t & binder)
{
    binder = BAD_SOCKET;

    if (nullptr == host || nullptr == service)
    {
        DBG_LOG("udp_bind failed: nullptr");
        return(false);
    }

    struct addrinfo addr_temp;
    memset(&addr_temp, 0, sizeof(addr_temp));
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
        socket_t bindfd = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
        if (BAD_SOCKET == bindfd)
        {
            DBG_LOG("socket failed: %d, try again", stupid_net_error());
            continue;
        }

        if (bind(bindfd, addr_info->ai_addr, addr_info->ai_addrlen) < 0)
        {
            RUN_LOG_ERR("bind failed: %d", stupid_net_error());
            stupid_close_socket(bindfd);
        }
        else
        {
            binder = bindfd;
            ret = true;
            break;
        }
    }

    freeaddrinfo(addr_dup);

    return(ret);
}

bool udp_connect(const char * host, const char * service, socket_t & connecter)
{
    connecter = BAD_SOCKET;

    if (nullptr == host || nullptr == service)
    {
        DBG_LOG("udp_connect failed: nullptr");
        return(false);
    }

    struct addrinfo addr_temp;
    memset(&addr_temp, 0, sizeof(addr_temp));
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
        socket_t connectfd = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
        if (BAD_SOCKET == connectfd)
        {
            DBG_LOG("socket failed: %d, try again", stupid_net_error());
            continue;
        }

        if (connect(connectfd, addr_info->ai_addr, addr_info->ai_addrlen) < 0)
        {
            RUN_LOG_ERR("connect failed: %d", stupid_net_error());
            stupid_close_socket(connectfd);
        }
        else
        {
            connecter = connectfd;
            ret = true;
            break;
        }
    }

    freeaddrinfo(addr_dup);

    return(ret);
}

NAMESPACE_STUPID_NET_END
