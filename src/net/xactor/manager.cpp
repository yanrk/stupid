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

#include "net/xactor/manager.h"
#include "net/xactor/connection.h"
#include "net/xactor/service_base.h"
#include "base/common/memory.h"
#include "base/log/log.h"

#ifdef _MSC_VER
    #include "net/xactor/proactor.h"
#else
    #include "net/xactor/reactor.h"
#endif // _MSC_VER

NAMESPACE_STUPID_NET_BEGIN

TcpManager::TcpManager()
    : m_running(false)
    , m_xactor(nullptr)
    , m_service(nullptr)
{
    STUPID_NEW(m_xactor, TcpXactor);
}

TcpManager::~TcpManager()
{
    STUPID_DEL(m_xactor);
    m_service = nullptr;
}

bool TcpManager::set_service(TcpServiceBase * service)
{
    if (nullptr == service || m_running)
    {
        return(false);
    }
    else
    {
        m_service = service;
        return(true);
    }
}

bool TcpManager::init(size_t event_thread_count, size_t handle_thread_count, unsigned short * service_port, size_t service_port_count)
{
    if (m_running)
    {
        RUN_LOG_CRI("tcp manager has started");
        return(false);
    }

    if (nullptr == m_xactor)
    {
        RUN_LOG_CRI("new tcp xactor failed: %d", stupid_system_error());
        return(false);
    }

    if (nullptr == m_service)
    {
        RUN_LOG_CRI("service is not configured");
        return(false);
    }

    if (!m_xactor->init(this, event_thread_count, handle_thread_count, service_port, service_port_count))
    {
        RUN_LOG_CRI("init tcp xactor failed");
        return(false);
    }

    m_running = true;

    RUN_LOG_DBG("init tcp manager success");

    return(true);
}

void TcpManager::exit()
{
    if (nullptr == m_xactor)
    {
        return;
    }

    m_xactor->exit();

    m_running = false;

    RUN_LOG_DBG("exit tcp manager success");
}

bool TcpManager::create_connection(const sockaddr_in_t & server_address, size_t identity, unsigned short bind_port)
{
    if (!m_running)
    {
        return(false);
    }
    return(m_xactor->create_connection(server_address, identity, bind_port));
}

bool TcpManager::handle_connect(TcpConnectionBase * connection, size_t identity)
{
    return(m_service->on_connect(connection, identity));
}

bool TcpManager::handle_accept(TcpConnectionBase * connection, unsigned short listener_port)
{
    return(m_service->on_accept(connection, listener_port));
}

bool TcpManager::handle_recv(TcpConnectionBase * connection)
{
    return(m_service->on_recv(connection));
}

bool TcpManager::handle_send(TcpConnectionBase * connection)
{
    return(m_service->on_send(connection));
}

bool TcpManager::handle_close(TcpConnectionBase * connection)
{
    return(m_service->on_close(connection));
}

NAMESPACE_STUPID_NET_END
