/********************************************************
 * Description : utility of net
 * Data        : 2014-02-28 11:44:56
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
    #include <netdb.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif // _MSC_VER

#include <cstring>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "base/log/log.h"
#include "base/string/splitter.h"
#include "net/utility/utility.h"

static bool check_ip_sub_element(const std::string & element)
{
    if (element.empty() || element.size() > 3)
    {
        return false;
    }
    else if ('0' == element[0] && element.size() > 1)
    {
        return false;
    }
    else if (element.end() != std::find_if_not(element.begin(), element.end(), isdigit))
    {
        return false;
    }
    else
    {
        if (atoi(element.c_str()) > 255)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

NAMESPACE_STUPID_NET_BEGIN

bool check_ip_format(const char * ip)
{
    if (nullptr == ip)
    {
        return false;
    }

    Stupid::Base::StringSplitter splitter(ip, "", ".");

    if (4 != splitter.count_bound())
    {
        return false;
    }

    while (splitter.has_element())
    {
        if (!check_ip_sub_element(splitter.pop_element()))
        {
            return false;
        }
    }

    return true;
}

bool transform_address(const char * ip, unsigned short port, sockaddr_in_t & address)
{
    if (!check_ip_format(ip))
    {
        DBG_LOG("transform_address failed: ip is invalid");
        return false;
    }

    if (!check_port_value(port))
    {
        DBG_LOG("transform_address failed: port is invalid");
        return false;
    }

    memset(&address, 0x00, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
#ifdef _MSC_VER
    address.sin_addr.s_addr = inet_addr(ip);
#else
    inet_aton(ip, &address.sin_addr);
#endif // _MSC_VER

    return true;
}

bool resolve_address(const char * host, const char * service, bool is_listen, bool is_tcp, std::list<sockaddr_in_t> & address_list)
{
    if (nullptr == host)
    {
        DBG_LOG("resolve_address failed: host is nullptr");
        return false;
    }

    struct addrinfo addr_temp;
    memset(&addr_temp, 0x00, sizeof(addr_temp));
    addr_temp.ai_flags = (is_listen ? AI_PASSIVE : 0);
    addr_temp.ai_family = AF_INET;
    addr_temp.ai_socktype = (is_tcp ? SOCK_STREAM : SOCK_DGRAM);

    struct addrinfo * addr_info = nullptr;
    if (0 != getaddrinfo(host, service, &addr_temp, &addr_info))
    {
        RUN_LOG_ERR("getaddrinfo failed: %d", stupid_net_error());
        return false;
    }

    struct addrinfo * addr_dup = addr_info;

    while (nullptr != addr_dup)
    {
        if (addr_temp.ai_family == addr_dup->ai_family && addr_temp.ai_socktype == addr_dup->ai_socktype)
        {
            sockaddr_in_t address;
            memset(&address, 0x00, sizeof(address));
            memcpy(&address, addr_dup->ai_addr, addr_dup->ai_addrlen);
            address_list.push_back(address);
        }
        addr_dup = addr_dup->ai_next;
    }

    freeaddrinfo(addr_info);

    return true;
}

NAMESPACE_STUPID_NET_END
