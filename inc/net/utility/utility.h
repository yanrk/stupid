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

#ifndef STUPID_NET_UTILITY_H
#define STUPID_NET_UTILITY_H


#include <list>
#include "net/common/common.h"

NAMESPACE_STUPID_NET_BEGIN

template <typename Integer>
bool check_port_value(Integer port)
{
    return(0 < port && 65536 > port);
}

STUPID_CXX_API(bool) check_ip_format(const char * ip);
STUPID_CXX_API(bool) transform_address(const char * ip, unsigned short port, sockaddr_in_t & address);
STUPID_CXX_API(bool) resolve_address(const char * host, const char * service, bool is_listen, bool is_tcp, std::list<sockaddr_in_t> & address_list);

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_UTILITY_H
