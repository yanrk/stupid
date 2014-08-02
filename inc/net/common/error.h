/********************************************************
 * Description : macros of stupid net errors
 * Data        : 2013-11-01 00:30:50
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_ERROR_H
#define STUPID_NET_ERROR_H


#ifdef _MSC_VER
    #define NET_ERR_EINTR             WSAEINTR
    #define NET_ERR_AGAIN             WSAEWOULDBLOCK
    #define NET_ERR_WOULDBLOCK        WSAEWOULDBLOCK
    #define NET_ERR_TIMEDOUT          WSAETIMEDOUT
#else
    #define NET_ERR_EINTR             EINTR
    #define NET_ERR_AGAIN             EAGAIN
    #define NET_ERR_WOULDBLOCK        EWOULDBLOCK
    #define NET_ERR_TIMEDOUT          ETIMEDOUT
#endif // _MSC_VER


#endif // STUPID_NET_ERROR_H
