/********************************************************
 * Description : tcp connection manager class
 * Data        : 2014-07-05 14:45:42
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_NET_MANAGER_H
#define STUPID_NET_MANAGER_H


#include "net/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_NET_BEGIN

class TcpConnectionBase;
class TcpServiceBase;
class TcpXactor;

class STUPID_API TcpManager : private Stupid::Base::Uncopy
{
public:
    TcpManager();
    ~TcpManager();

public:
    bool set_service(TcpServiceBase * service);

public:
    bool init(size_t event_thread_count = 0, size_t handle_thread_count = 5, unsigned short * service_port = nullptr, size_t service_port_count = 0);
    void exit();

public:
    bool create_connection(const sockaddr_in_t & server_address, size_t identity, const char * bind_ip = "0.0.0.0", unsigned short bind_port = 0);

private:
    friend class TcpXactor;

private:
    bool handle_connect(TcpConnectionBase * connection, size_t identity);
    bool handle_accept(TcpConnectionBase * connection, unsigned short listener_port);
    bool handle_recv(TcpConnectionBase * connection);
    bool handle_send(TcpConnectionBase * connection);
    bool handle_close(TcpConnectionBase * connection);

private:
    volatile bool         m_running;
    TcpXactor           * m_xactor;
    TcpServiceBase      * m_service;
};

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_MANAGER_H
