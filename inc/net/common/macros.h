/********************************************************
 * Description : macros of stupid net
 * Data        : 2013-10-30 00:01:10
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_MACROS_H
#define STUPID_NET_MACROS_H


#ifdef _MSC_VER
    #include <winsock2.h>
#else
    #include <errno.h>
#endif // _MSC_VER

#ifdef _MSC_VER
    #define BAD_SOCKET                           (INVALID_SOCKET)
    #define stupid_close_socket(sock)            ::closesocket(sock)
    #define stupid_system_error()                (::GetLastError())
    #define stupid_net_error()                   (::WSAGetLastError())
#else
    #define BAD_SOCKET                           (-1)
    #define stupid_close_socket(sock)            ::close(sock)
    #define stupid_system_error()                (errno + 0)
    #define stupid_net_error()                   (errno + 0)
#endif // _MSC_VER

#define stupid_send(sock, data, data_len)        ::send(sock, data, data_len, 0)
#define stupid_recv(sock, buff, buff_siz)        ::recv(sock, buff, buff_siz, 0)

#define stupid_is_net_blocking_error()           \
    (                                            \
        NET_ERR_EINTR == stupid_net_error() ||   \
        NET_ERR_AGAIN == stupid_net_error() ||   \
        NET_ERR_WOULDBLOCK == stupid_net_error() \
    )


#endif // STUPID_NET_MACROS_H
