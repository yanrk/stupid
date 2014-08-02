/********************************************************
 * Description : tcp connection manager class
 * Data        : 2014-07-05 14:45:42
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_MANAGER_H
#define STUPID_NET_MANAGER_H


#include "net/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_NET_BEGIN

class TcpConnectionBase;
class TcpServiceBase;

#ifdef _MSC_VER
    class TcpProactor;
    typedef TcpProactor   TcpXactor;
#else
    class TcpReactor;
    typedef TcpReactor    TcpXactor;
#endif // _MSC_VER

class STUPID_API TcpManager : private Stupid::Base::Uncopy
{
public:
    TcpManager();
    ~TcpManager();

public:
    bool set_service(TcpServiceBase * service);

public:
    bool init(size_t handle_thread_count = 5, unsigned short service_port = 0);
    void exit();

public:
    bool create_connection(const sockaddr_in_t & server_address, size_t identity);

private:
#ifdef _MSC_VER
    friend class TcpProactor;
#else
    friend class TcpReactor;
#endif // _MSC_VER

private:
    bool handle_connect(TcpConnectionBase * connection, size_t identity);
    bool handle_accept(TcpConnectionBase * connection);
    bool handle_recv(TcpConnectionBase * connection);
    bool handle_send(TcpConnectionBase * connection);
    bool handle_close(TcpConnectionBase * connection);

private:
    bool                  m_running;
    TcpXactor           * m_xactor;
    TcpServiceBase      * m_service;
};

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_MANAGER_H
