/********************************************************
 * Description : typedefs of stupid net
 * Data        : 2013-10-30 00:26:41
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_NET_TYPEDEFS_H
#define STUPID_NET_TYPEDEFS_H


#ifdef _MSC_VER
    #include <winsock2.h>
#else
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif // _MSC_VER

#ifdef _MSC_VER
    typedef SOCKET               socket_t;
    typedef int                  sockaddr_len_t;
    typedef SOCKADDR             sockaddr_t;
    typedef SOCKADDR_IN          sockaddr_in_t;
#else
    typedef int                  socket_t;
    typedef socklen_t            sockaddr_len_t;
    typedef struct sockaddr      sockaddr_t;
    typedef struct sockaddr_in   sockaddr_in_t;
#endif // _MSC_VER


#endif // STUPID_NET_TYPEDEFS_H
