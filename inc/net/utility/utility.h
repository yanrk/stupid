/********************************************************
 * Description : utility of net
 * Data        : 2014-02-28 11:44:56
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_UTILITY_H
#define STUPID_NET_UTILITY_H


#include "net/common/common.h"

NAMESPACE_STUPID_NET_BEGIN

template <typename Integer>
bool check_port_value(Integer port)
{
    return(0 < port && 65536 > port);
}

STUPID_CXX_API(bool) check_ip_format(const char * ip);
STUPID_CXX_API(bool) transform_address(const char * ip, unsigned short port, sockaddr_in_t & address);

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_UTILITY_H
