/********************************************************
 * Description : service base class
 * Data        : 2014-07-05 15:43:12
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_SERVICE_BASE_H
#define STUPID_NET_SERVICE_BASE_H


#include "net/common/common.h"

NAMESPACE_STUPID_NET_BEGIN

class TcpConnectionBase;

class STUPID_API TcpServiceBase
{
public:
    TcpServiceBase();
    virtual ~TcpServiceBase();

public:
    virtual bool on_connect(TcpConnectionBase * connection, size_t identity) = 0;
    virtual bool on_accept(TcpConnectionBase * connection) = 0;
    virtual bool on_recv(TcpConnectionBase * connection) = 0;
    virtual bool on_send(TcpConnectionBase * connection) = 0;
    virtual bool on_close(TcpConnectionBase * connection) = 0;
};

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_SERVICE_BASE_H
