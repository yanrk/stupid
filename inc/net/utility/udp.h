/********************************************************
 * Description : simple encapsulations of udp socket
 * Data        : 2013-11-10 21:36:20
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_UDP_H
#define STUPID_NET_UDP_H


#include "net/common/common.h"

NAMESPACE_STUPID_NET_BEGIN

STUPID_CXX_API(bool) udp_bind(const char * host, const char * service, socket_t & listener);
STUPID_CXX_API(bool) udp_connect(const char * host, const char * service, socket_t & connecter);

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_UDP_H
