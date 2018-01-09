/********************************************************
 * Description : windows tcp connection proactor class
 * Data        : 2014-07-03 10:49:42
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#if defined(_MSC_VER) && !defined(XACTOR_USE_SELECT)


#include <algorithm>
#include "net/xactor/proactor.h"
#include "net/xactor/manager.h"
#include "net/utility/tcp.h"
#include "net/utility/utility.h"
#include "base/log/log.h"
#include "base/time/time.h"
#include "base/utility/guard.h"
#include "base/utility/singleton.h"

NAMESPACE_STUPID_NET_BEGIN

struct PROACTOR_THREAD_PARAM
{
    TcpXactor & tcp_xactor;
    size_t      thread_index;

    PROACTOR_THREAD_PARAM(TcpXactor & xactor, size_t index)
        : tcp_xactor(xactor), thread_index(index)
    {

    }
};

thread_return_t STUPID_STDCALL connection_thread_process(thread_argument_t param)
{
    if (nullptr != param)
    {
        PROACTOR_THREAD_PARAM * thread_param = reinterpret_cast<PROACTOR_THREAD_PARAM *>(param);
        thread_param->tcp_xactor.proactor_connection_process(thread_param->thread_index);
        STUPID_DEL(thread_param);
    }
    return(THREAD_DEFAULT_RET);
}

thread_return_t STUPID_STDCALL business_thread_process(thread_argument_t param)
{
    if (nullptr != param)
    {
        PROACTOR_THREAD_PARAM * thread_param = reinterpret_cast<PROACTOR_THREAD_PARAM *>(param);
        thread_param->tcp_xactor.proactor_business_process(thread_param->thread_index);
        STUPID_DEL(thread_param);
    }
    return(THREAD_DEFAULT_RET);
}

static LPFN_ACCEPTEX               AcceptExFunc = nullptr;
static LPFN_GETACCEPTEXSOCKADDRS   GetAcceptExSockAddrsFunc = nullptr;

static bool get_extern_function_pointer(socket_t sockfd, GUID & guid, LPVOID output_buf, DWORD output_siz)
{
    DWORD  control_code = SIO_GET_EXTENSION_FUNCTION_POINTER;
    LPVOID input_buf = &guid;
    DWORD  input_siz = sizeof(guid);
    DWORD  output_len = 0;
    return(SOCKET_ERROR != WSAIoctl(sockfd, control_code, input_buf, input_siz, output_buf, output_siz, &output_len, nullptr, nullptr));
}

static bool get_extern_wsa_functions(socket_t sockfd)
{
    if (nullptr == AcceptExFunc)
    {
        GUID acceptex_guid = WSAID_ACCEPTEX;
        if (!get_extern_function_pointer(sockfd, acceptex_guid, &AcceptExFunc, sizeof(AcceptExFunc)))
        {
            RUN_LOG_CRI("get AcceptExFunc failed: %d", stupid_net_error());
            return(false);
        }
    }

    if (nullptr == GetAcceptExSockAddrsFunc)
    {
        GUID sockaddrs_guid = WSAID_GETACCEPTEXSOCKADDRS;
        if (!get_extern_function_pointer(sockfd, sockaddrs_guid, &GetAcceptExSockAddrsFunc, sizeof(GetAcceptExSockAddrsFunc)))
        {
            RUN_LOG_CRI("get GetAcceptExSockAddrsFunc failed: %d", stupid_net_error());
            return(false);
        }
    }

    return(nullptr != AcceptExFunc && nullptr != GetAcceptExSockAddrsFunc);
}

static bool tcp_async_listen(unsigned short port, socket_t & listener, int backlog)
{
    sockaddr_in_t server_address;
    if (!transform_address("0.0.0.0", port, server_address))
    {
        RUN_LOG_ERR("transform_address failed");
        return(false);
    }

    listener = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (BAD_SOCKET == listener)
    {
        RUN_LOG_ERR("WSASocket failed: %d", stupid_net_error());
        return(false);
    }

    do
    {
        const int reuse_addr_on = 1;
        const char * reuse_addr_ptr = reinterpret_cast<const char *>(&reuse_addr_on);
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, reuse_addr_ptr, sizeof(reuse_addr_on)) < 0)
        {
            RUN_LOG_ERR("setsockopt(reuse-addr) failed: %d", stupid_net_error());
            break;
        }

        if (bind(listener, reinterpret_cast<sockaddr_t *>(&server_address), sizeof(server_address)) < 0)
        {
            RUN_LOG_ERR("bind failed: %d", stupid_net_error());
            break;
        }

        if (listen(listener, backlog) < 0)
        {
            RUN_LOG_ERR("listen failed: %d", stupid_net_error());
            break;
        }

        return(true);
    } while (false);

    tcp_close(listener);

    return(false);
}

enum iocp_event_enum
{
    exit_iocp = 0, 
    accept_iocp, 
    recv_iocp, 
    send_iocp  
};

enum business_event_enum
{
    connect_notify, 
    accept_notify, 
    recv_notify, 
    send_notify, 
    close_notify 
};

TcpXactor::TcpXactor()
    : m_running(false)
    , m_manager(nullptr)
    , m_iocp(nullptr)
    , m_listeners()
    , m_thread(nullptr)
    , m_thread_count(0)
    , m_connection_thread_count(0)
    , m_block_pool()
    , m_business_event_list_vector()
    , m_business_event_list_vector_locker()
    , m_normal_connection_set()
    , m_closed_connection_set()
    , m_connection_set_locker()
    , m_unique_creator(0, ~static_cast<size_t>(0))
    , m_binded_connection_set()
    , m_binded_connection_set_locker()
{

}

TcpXactor::~TcpXactor()
{
    exit();
}

bool TcpXactor::init(TcpManager * manager, size_t event_thread_count, size_t handle_thread_count, unsigned short * service_port, size_t service_port_count)
{
    if (nullptr == manager)
    {
        RUN_LOG_CRI("tcp manager is nullptr");
        return(false);
    }

    exit();

    m_running = true;

    m_manager = manager;

    if (0 == event_thread_count)
    {
        calc_event_thread_count(event_thread_count);
    }
    m_connection_thread_count = event_thread_count;

    if (!create_iocp())
    {
        return(false);
    }
    else
    {
        RUN_LOG_DBG("create iocp success");
    }

    if (!create_listener(service_port, service_port_count))
    {
        return(false);
    }
    else if (0 != service_port)
    {
        RUN_LOG_DBG("create listener success");
    }

    if (!acquire_proactor_threads(handle_thread_count))
    {
        return(false);
    }
    else
    {
        RUN_LOG_DBG("create proactor threads success");
    }

    RUN_LOG_DBG("init tcp proactor success");

    return(true);
}

void TcpXactor::exit()
{
    if (!m_running)
    {
        return;
    }

    m_running = false;

    release_proactor_threads();
    destroy_listener();
    destroy_iocp();
    destroy_connections();
    clear_business_event();

    m_block_pool.clear();
    m_unique_creator.reset();

    m_manager = nullptr;

    RUN_LOG_DBG("exit tcp proactor success");
}

bool TcpXactor::create_connection(const sockaddr_in_t & server_address, size_t identity, const char * bind_ip, unsigned short bind_port)
{
    return(do_connect(server_address, identity, bind_ip, bind_port));
}

void TcpXactor::connection_send(TcpConnection * connection)
{
    /* when send-buffer has data, the connection should continue to send */
    do
    {
        /* data has been sent in do_send() */
        if (0 == connection->send_buffer_size())
        {
            return;
        }

        iocp_event * post_event = &connection->m_async_send;

        /* wait do_send finish */
        while (post_event->buffer != post_event->data.buf || 0 != post_event->data.len)
        {
            if (connection->get_error())
            {
                break;
            }
            Stupid::Base::stupid_ms_sleep(10);
        }

        /* data has been sent in do_send() */
        if (0 == connection->send_buffer_size())
        {
            return;
        }

        size_t send_len = std::min<size_t>(post_event->buffer_size, connection->send_buffer_size());
        post_event->data.buf = post_event->buffer;
        post_event->data.len = static_cast<ULONG>(send_len);
        if (!connection->send_buffer_copy_len(post_event->buffer, send_len))
        {
            RUN_LOG_TRK("connection_send failed: send_buffer_copy_len error"); /* RUN_LOG_ERR */
            break;
        }

        if (!post_send(post_event))
        {
            RUN_LOG_TRK("connection_send failed: post_send error"); /* RUN_LOG_ERR */
            break;
        }

        return;
    } while (false);

    do_close(connection);
}

void TcpXactor::close_connection(TcpConnection * connection)
{
    /* binded-set make sure we will not close connection more times */
    if (delete_connection_from_iocp(connection))
    {
        connection->set_error();

        BusinessEvent business_event;
        business_event.connection = connection;
        business_event.event = close_notify;
        connection->increase_reference();
        append_business_event(business_event);
    }
}

bool TcpXactor::running()
{
    return(m_running);
}

void TcpXactor::calc_event_thread_count(size_t & event_thread_count)
{
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    event_thread_count = system_info.dwNumberOfProcessors;
}

bool TcpXactor::create_listener(unsigned short * service_port, size_t service_port_count)
{
    if (nullptr == service_port || 0 == service_port_count)
    {
        return(true);
    }

    for (size_t service_port_index = 0; service_port_index < service_port_count; ++service_port_index)
    {
        unsigned short port = service_port[service_port_index];
        if (0 == port)
        {
            continue;
        }

        TcpConnection * connection = acquire_connection();
        if (nullptr == connection)
        {
            RUN_LOG_ERR("acquire connection failed: %d", stupid_system_error());
            return(false);
        }

        socket_t listener = BAD_SOCKET;
        if (!tcp_async_listen(port, listener, SOMAXCONN))
        {
            release_connection(connection);
            return(false);
        }

        tcp_set_block_switch(listener, false);

        connection->set_socket(listener);

        if (!get_extern_wsa_functions(listener))
        {
            release_connection(connection);
            return(false);
        }

        insert_connection(connection);

        if (!append_connection_to_iocp(connection))
        {
            remove_connection(connection);
            return(false);
        }

        m_listeners.push_back(connection);

        for (size_t thread_index = 0; thread_index < m_connection_thread_count; ++thread_index)
        {
            connection = acquire_connection();
            if (nullptr == connection)
            {
                RUN_LOG_ERR("acquire connection failed: %d", stupid_system_error());
                return(false);
            }

            connection->set_listener(listener);
            connection->set_listener_port(port);

            if (!post_accept(&connection->m_async_recv))
            {
                RUN_LOG_ERR("post_accept failed");
                release_connection(connection);
                return(false);
            }

            insert_connection(connection);
        }
    }

    return(true);
}

void TcpXactor::destroy_listener()
{
    ConnectionVector::iterator iter = m_listeners.begin();
    while (m_listeners.end() != iter)
    {
        delete_connection_from_iocp(*iter);
        ++iter;
    }
    m_listeners.clear();
}

bool TcpXactor::create_iocp()
{
    m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    if (nullptr == m_iocp)
    {
        RUN_LOG_CRI("create iocp failed: %d", stupid_net_error());
        return(false);
    }
    return(true);
}

void TcpXactor::destroy_iocp()
{
    if (nullptr != m_iocp)
    {
        CloseHandle(m_iocp);
        m_iocp = nullptr;
    }
}

bool TcpXactor::append_connection_to_iocp(TcpConnection * connection)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> binded_connection_set_guard(m_binded_connection_set_locker);
    if (m_binded_connection_set.end() != m_binded_connection_set.find(connection))
    {
        return(false);
    }

    if (nullptr == CreateIoCompletionPort(reinterpret_cast<HANDLE>(connection->get_socket()), m_iocp, reinterpret_cast<ULONG_PTR>(connection), 0))
    {
        RUN_LOG_CRI("append connection to iocp failed: %d", stupid_net_error());
        return(false);
    }

    size_t unique_value = 0;
    m_unique_creator.acquire(unique_value);
    connection->set_unique(unique_value);

    connection->increase_reference();

    m_binded_connection_set.insert(connection);

    return(true);
}

bool TcpXactor::delete_connection_from_iocp(TcpConnection * connection)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> binded_connection_set_guard(m_binded_connection_set_locker);
    if (m_binded_connection_set.end() == m_binded_connection_set.find(connection))
    {
        return(false);
    }

    m_unique_creator.release(connection->get_unique());

    m_binded_connection_set.erase(connection);

    connection->decrease_reference();

    return(true);
}

bool TcpXactor::post_accept(iocp_event * post_event)
{
    socket_t accepter = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (INVALID_SOCKET == accepter)
    {
        RUN_LOG_ERR("WSASocket failed: %d", stupid_net_error());
        return(false);
    }

    socket_t sockfd = post_event->connection->get_socket();
    tcp_close(sockfd);

    post_event->connection->set_socket(accepter);
    post_event->post_type = accept_iocp;

    socket_t listener = post_event->connection->get_listener();
    DWORD  addr_reserve_len = sizeof(sockaddr_in_t) + 16;
    char * buffer = post_event->buffer;
    DWORD  buffer_size = 0; /* static_cast<DWORD>(post_event->buffer_size - 2 * addr_reserve_len); */
    DWORD recv_len = 0;
    if (!AcceptExFunc(listener, accepter, buffer, buffer_size, addr_reserve_len, addr_reserve_len, &recv_len, &post_event->overlapped))
    {
        if (WSA_IO_PENDING != stupid_net_error())
        {
            RUN_LOG_ERR("AcceptEx failed: %d", stupid_net_error());
            return(false);
        }
    }

    return(true);
}

bool TcpXactor::post_recv(iocp_event * post_event)
{
    TcpConnection * connection = post_event->connection;
    WSABUF buffer_array[1] = { post_event->data };
    DWORD recv_len = 0;
    DWORD recv_flg = 0;

    if (SOCKET_ERROR == WSARecv(connection->get_socket(), buffer_array, 1, &recv_len, &recv_flg, &post_event->overlapped, nullptr))
    {
        if (WSA_IO_PENDING != stupid_net_error())
        {
            RUN_LOG_ERR("WSARecv failed: %d", stupid_net_error());
            return(false);
        }
    }

    return(true);
}

bool TcpXactor::post_send(iocp_event * post_event)
{
    TcpConnection * connection = post_event->connection;
    WSABUF buffer_array[1] = { post_event->data };
    DWORD send_len = 0;
    DWORD send_flg = 0;

    if (SOCKET_ERROR == WSASend(connection->get_socket(), buffer_array, 1, &send_len, send_flg, &post_event->overlapped, nullptr))
    {
        if (WSA_IO_PENDING != stupid_net_error())
        {
            RUN_LOG_ERR("WSASend failed: %d", stupid_net_error());
            return(false);
        }
    }

    return(true);
}

void TcpXactor::post_exit()
{
    for (size_t thread_index = 0; thread_index < m_connection_thread_count; ++thread_index)
    {
        if (!PostQueuedCompletionStatus(m_iocp, 0, static_cast<ULONG_PTR>(exit_iocp), nullptr))
        {
            RUN_LOG_CRI("PostQueuedCompletionStatus failed: %d", stupid_net_error());
        }
    }
}

bool TcpXactor::do_connect(const sockaddr_in_t & server_address, size_t identity, const char * bind_ip, unsigned short bind_port)
{
    socket_t connecter = BAD_SOCKET;
    if (!tcp_connect(server_address, connecter, bind_ip, bind_port))
    {
        return(false);
    }

    tcp_set_block_switch(connecter, false);

    TcpConnection * connection = acquire_connection();
    if (nullptr == connection)
    {
        RUN_LOG_ERR("acquire connection failed: %d", stupid_system_error());
        tcp_close(connecter);
        return(false);
    }

    connection->set_socket(connecter);
    connection->set_connected(true);
    connection->set_requester(true);
    connection->set_address(server_address);
    connection->set_identity(identity);
    connection->m_async_recv.post_type = recv_iocp;
    connection->m_async_send.post_type = send_iocp;

    insert_connection(connection);

    if (!append_connection_to_iocp(connection))
    {
        remove_connection(connection);
        return(false);
    }

    BusinessEvent business_event;
    business_event.connection = connection;

    business_event.event = connect_notify;
    connection->increase_reference();
    append_business_event(business_event);
    /*
    business_event.event = send_notify;
    connection->increase_reference();
    append_business_event(business_event);
    */
    return(post_recv(&connection->m_async_recv));
}

bool TcpXactor::do_accept(iocp_event * post_event, size_t data_len)
{
    socket_t listener = post_event->connection->get_listener();
    unsigned short listener_port = post_event->connection->get_listener_port();

    do
    {
        sockaddr_in_t * server_address = nullptr;
        sockaddr_in_t * client_address = nullptr;
        int server_addr_len = 0;
        int client_addr_len = 0;

        DWORD  addr_reserve_len = sizeof(sockaddr_in_t) + 16;
        char * buffer = post_event->buffer;
        DWORD  buffer_size = 0; /* static_cast<DWORD>(post_event->buffer_size - 2 * addr_reserve_len); */
        sockaddr_t ** srv_addr_ptr = reinterpret_cast<sockaddr_t **>(&server_address);
        sockaddr_t ** clt_addr_ptr = reinterpret_cast<sockaddr_t **>(&client_address);

        GetAcceptExSockAddrsFunc(buffer, buffer_size, addr_reserve_len, addr_reserve_len, srv_addr_ptr, &server_addr_len, clt_addr_ptr, &client_addr_len);

        socket_t accepter = post_event->connection->get_socket();
        post_event->connection->set_socket(BAD_SOCKET);

        tcp_set_block_switch(accepter, false);

        TcpConnection * connection = acquire_connection();
        if (nullptr == connection)
        {
            RUN_LOG_ERR("acquire connection failed: %d", stupid_system_error());
            tcp_close(accepter);
            break;
        }

        connection->set_socket(accepter);
        connection->set_listener(listener);
        connection->set_listener_port(listener_port);
        connection->set_connected(true);
        connection->set_requester(false);
        connection->set_address(*client_address);
        connection->m_async_recv.post_type = recv_iocp;
        connection->m_async_send.post_type = send_iocp;

        insert_connection(connection);

        if (!append_connection_to_iocp(connection))
        {
            remove_connection(connection);
            break;
        }

        BusinessEvent business_event;
        business_event.connection = connection;

        business_event.event = accept_notify;
        connection->increase_reference();
        append_business_event(business_event);
        /*
        business_event.event = send_notify;
        connection->increase_reference();
        append_business_event(business_event);
        */
        if (data_len > 0)
        {
            memcpy(connection->m_async_recv.buffer, buffer, data_len);
            if (!do_recv(&connection->m_async_recv, data_len))
            {
                do_close(connection);
            }
        }
        else
        {
            if (!post_recv(&connection->m_async_recv))
            {
                do_close(connection);
            }
        }
    } while (false);

    return(post_accept(post_event));
}

bool TcpXactor::do_recv(iocp_event * post_event, size_t data_len)
{
    TcpConnection * connection = post_event->connection;

    if (!connection->recv_buffer_fill_len(post_event->buffer, data_len))
    {
        RUN_LOG_TRK("do_recv failed: recv_buffer_fill_len error"); /* RUN_LOG_ERR */
        return(false);
    }

    if (connection->recv_buffer_size() >= connection->recv_buffer_water_mark())
    {
        BusinessEvent business_event;
        business_event.connection = connection;
        business_event.event = recv_notify;
        connection->increase_reference();
        append_business_event(business_event);
    }

    return(post_recv(post_event));
}

bool TcpXactor::do_send(iocp_event * post_event, size_t data_len)
{
    TcpConnection * connection = post_event->connection;

    if (!connection->send_buffer_drop_len(data_len))
    {
        RUN_LOG_TRK("do_send failed: send_buffer_drop_len error"); /* RUN_LOG_ERR */
        return(false);
    }

    if (data_len == post_event->data.len)
    {
        size_t send_len = std::min<size_t>(post_event->buffer_size, connection->send_buffer_size());
        post_event->data.buf = post_event->buffer;
        post_event->data.len = static_cast<ULONG>(send_len);
        if (0 != send_len)
        {
            if (!connection->send_buffer_copy_len(post_event->buffer, send_len))
            {
                RUN_LOG_TRK("do_send failed: send_buffer_copy_len error"); /* RUN_LOG_ERR */
                return(false);
            }
        }
        else
        {
            BusinessEvent business_event;
            business_event.connection = connection;
            business_event.event = send_notify;
            connection->increase_reference();
            append_business_event(business_event);
            return(true);
        }
    }
    else if (data_len < post_event->data.len)
    {
        post_event->data.buf += data_len;
        post_event->data.len -= static_cast<ULONG>(data_len);
    }
    else
    {
        RUN_LOG_TRK("do_send failed"); /* RUN_LOG_ERR */
        return(false);
    }

    return(post_send(post_event));
}

void TcpXactor::do_close(TcpConnection * connection)
{
    close_connection(connection);
}

bool TcpXactor::handle_connect(TcpConnection * connection, size_t identity)
{
    return(m_manager->handle_connect(connection, identity));
}

bool TcpXactor::handle_accept(TcpConnection * connection, unsigned short listener_port)
{
    return(m_manager->handle_accept(connection, listener_port));
}

bool TcpXactor::handle_recv(TcpConnection * connection)
{
    return(m_manager->handle_recv(connection));
}

bool TcpXactor::handle_send(TcpConnection * connection)
{
    return(m_manager->handle_send(connection));
}

bool TcpXactor::handle_close(TcpConnection * connection)
{
    return(m_manager->handle_close(connection));
}

void TcpXactor::handle_error(iocp_event * post_event)
{
    if (accept_iocp == post_event->post_type)
    {
        post_accept(post_event);
    }
    else
    {
        if (ERROR_NETNAME_DELETED != stupid_net_error() && ERROR_CONNECTION_ABORTED != stupid_net_error())
        {
            RUN_LOG_ERR("handle error: %d", stupid_net_error());
        }
        do_close(post_event->connection);
    }
}

bool TcpXactor::acquire_proactor_threads(size_t handle_thread_count)
{
    const size_t connection_thread_count = m_connection_thread_count;
    const size_t business_thread_count = handle_thread_count;
    const size_t thread_count = connection_thread_count + business_thread_count;

    STUPID_NEW_ARR(m_thread, Stupid::Base::Thread, thread_count);
    if (nullptr == m_thread)
    {
        RUN_LOG_CRI("new proactor thread array failed");
        return(false);
    }

    m_business_event_list_vector.resize(business_thread_count);

    for (m_thread_count = 0; m_thread_count < thread_count; ++m_thread_count)
    {
        size_t thread_index = 0;
        thread_func_ptr_t proactor_thread_process = nullptr;
        if (m_thread_count < connection_thread_count)
        {
            proactor_thread_process = connection_thread_process;
            thread_index = m_thread_count + 1;
        }
        else
        {
            proactor_thread_process = business_thread_process;
            thread_index = m_thread_count - connection_thread_count + 1;
        }

        PROACTOR_THREAD_PARAM * proactor_thread_param = nullptr;
        STUPID_NEW(proactor_thread_param, PROACTOR_THREAD_PARAM(*this, thread_index));
        if (nullptr == proactor_thread_param)
        {
            RUN_LOG_CRI("new proactor thread param failed");
            return(false);
        }

        Stupid::Base::Thread & proactor_thread = m_thread[m_thread_count];
        proactor_thread.set_thread_args(proactor_thread_process, proactor_thread_param);
        if (!proactor_thread.acquire())
        {
            RUN_LOG_CRI("create proactor thread failed: %d", stupid_system_error());
            STUPID_DEL(proactor_thread_param);
            return(false);
        }
    }

    return(true);
}

void TcpXactor::release_proactor_threads()
{
    post_exit();

    for (size_t thread_index = 0; thread_index < m_thread_count; ++thread_index)
    {
        Stupid::Base::Thread & proactor_thread = m_thread[thread_index];
        proactor_thread.release();
    }
    STUPID_DEL_ARR(m_thread);
    m_thread_count = 0;
    m_connection_thread_count = 0;
}

TcpConnection * TcpXactor::acquire_connection()
{
    TcpConnection * connection = nullptr;
    STUPID_NEW(connection, TcpConnection(*this, m_block_pool));
    return(connection);
}

void TcpXactor::release_connection(TcpConnection *& connection)
{
    STUPID_DEL(connection);
}

void TcpXactor::insert_connection(TcpConnection * connection)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> connection_set_guard(m_connection_set_locker);
    m_normal_connection_set.insert(connection);
    if (connection->get_connected())
    {
        sockaddr_in_t remote_address = connection->get_address();
        if (connection->get_requester())
        {
            RUN_LOG_TRK("connect to server %s:%d, connection: %x", inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port), connection);
        }
        else
        {
            RUN_LOG_TRK("client %s:%d connect, connection: %x", inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port), connection);
        }
    }
}

void TcpXactor::remove_connection(TcpConnection *& connection)
{
    /*
     * when get to here, maybe connection->has_reference() is true
     * so we can not call release_connection(connection) here
     * but the connection will from normal-set transfer to closed-set
     */
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> connection_set_guard(m_connection_set_locker);
    m_normal_connection_set.erase(connection);
    m_closed_connection_set.insert(connection);
    if (connection->get_connected())
    {
        sockaddr_in_t remote_address = connection->get_address();
        if (connection->get_requester())
        {
            RUN_LOG_TRK("disconnect from server %s:%d, connection: %x", inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port), connection);
        }
        else
        {
            RUN_LOG_TRK("client %s:%d disconnect, connection: %x", inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port), connection);
        }
    }
}

void TcpXactor::destroy_invalid_connections()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> connection_set_guard(m_connection_set_locker);
    ConnectionSet valid_connection_set;
    ConnectionSet::iterator iter = m_closed_connection_set.begin();
    while (m_closed_connection_set.end() != iter)
    {
        TcpConnection * connection = *iter;
        if (connection->has_reference())
        {
            valid_connection_set.insert(connection);
        }
        else
        {
            release_connection(connection);
        }
        ++iter;
    }
    m_closed_connection_set.swap(valid_connection_set);
}

void TcpXactor::destroy_normal_connections()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> connection_set_guard(m_connection_set_locker);
    ConnectionSet::iterator iter = m_normal_connection_set.begin();
    while (m_normal_connection_set.end() != iter)
    {
        TcpConnection * connection = *iter;
        release_connection(connection);
        ++iter;
    }
    m_normal_connection_set.clear();
}

void TcpXactor::destroy_closed_connections()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> connection_set_guard(m_connection_set_locker);
    ConnectionSet::iterator iter = m_closed_connection_set.begin();
    while (m_closed_connection_set.end() != iter)
    {
        TcpConnection * connection = *iter;
        release_connection(connection);
        ++iter;
    }
    m_closed_connection_set.clear();
}

void TcpXactor::destroy_binded_connections()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> binded_connection_set_guard(m_binded_connection_set_locker);
    m_binded_connection_set.clear();
}

void TcpXactor::destroy_connections()
{
    destroy_normal_connections();
    destroy_closed_connections();
    destroy_binded_connections();
}

void TcpXactor::append_business_event(BusinessEvent & business_event)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> business_event_guard(m_business_event_list_vector_locker);
    const size_t business_thread_count = m_business_event_list_vector.size();
    TcpConnection * connection = business_event.connection;
    BusinessEventList & business_event_list = m_business_event_list_vector[connection->get_unique() % business_thread_count];
    business_event_list.push_back(business_event);
}

void TcpXactor::delete_business_event(size_t thread_index, BusinessEventList & event_list)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> business_event_guard(m_business_event_list_vector_locker);
    BusinessEventList & business_event_list = m_business_event_list_vector[thread_index - 1];
    event_list.splice(event_list.end(), business_event_list);
}

void TcpXactor::clear_business_event()
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> business_event_guard(m_business_event_list_vector_locker);
    m_business_event_list_vector.clear();
}

void TcpXactor::proactor_connection_process(size_t thread_index)
{
    RUN_LOG_DBG("proactor connection thread-%d begin", thread_index);

    while (running())
    {
        bool good = false;
        DWORD data_len = 0;
        OVERLAPPED * overlapped = nullptr;
        TcpConnection * connection = nullptr;

        if (GetQueuedCompletionStatus(m_iocp, &data_len, reinterpret_cast<PULONG_PTR>(&connection), &overlapped, INFINITE))
        {
            good = true;
        }

        if (good && exit_iocp == reinterpret_cast<ULONG_PTR>(connection))
        {
            RUN_LOG_DBG("get exit request and break from thread-%d", thread_index);
            break;
        }

        if (nullptr == overlapped)
        {
            continue;
        }

        {
            Stupid::Base::Guard<Stupid::Base::ThreadLocker> connection_set_guard(m_connection_set_locker);
            if (m_normal_connection_set.end() != m_normal_connection_set.find(connection))
            {
                connection->increase_reference();
            }
            else
            {
                continue;
            }
        }

        iocp_event * post_event = CONTAINING_RECORD(overlapped, iocp_event, overlapped);
        if (nullptr == post_event)
        {
            RUN_LOG_CRI("post_event is nullptr");
            connection->decrease_reference();
            continue;
        }
        else if (nullptr == post_event->connection)
        {
            RUN_LOG_CRI("post_event->connection is nullptr");
            connection->decrease_reference();
            continue;
        }

        if (!good)
        {
            handle_error(post_event);
            connection->decrease_reference();
            continue;
        }

        /*
         * when accept_iocp == post_event->post_type
         * m_listeners.end() != std::find(m_listeners.begin(), m_listeners.end(), connection)
         * otherwise, connection == post_event->connection
         */
        if (accept_iocp != post_event->post_type)
        {
            if (connection != post_event->connection)
            {
                RUN_LOG_CRI("accept_iocp != post_event->post_type && connection != post_event->connection");
                do_close(connection);
                connection->decrease_reference();
                continue;
            }
            if (0 == data_len)
            {
                do_close(connection);
                connection->decrease_reference();
                continue;
            }
        }

        switch (post_event->post_type)
        {
            case accept_iocp:
            {
                do_accept(post_event, data_len);
                good = true;
                break;
            }
            case recv_iocp:
            {
                good = do_recv(post_event, data_len);
                break;
            }
            case send_iocp:
            {
                good = do_send(post_event, data_len);
                break;
            }
            default:
            {
                RUN_LOG_ERR("unknown iocp post type: %d", post_event->post_type);
                good = false;
                break;
            }
        }

        if (!good)
        {
            do_close(connection);
        }

        connection->decrease_reference();
    }

    RUN_LOG_DBG("proactor connection thread-%d end", thread_index);
}

void TcpXactor::proactor_business_process(size_t thread_index)
{
    RUN_LOG_DBG("proactor business thread-%d begin", thread_index);

    BusinessEventList business_event_list;

    while (running())
    {
        destroy_invalid_connections();

        delete_business_event(thread_index, business_event_list);

        if (business_event_list.empty())
        {
            Stupid::Base::stupid_ms_sleep(10);
            continue;
        }

        BusinessEventList::iterator iter = business_event_list.begin();
        while (business_event_list.end() != iter && running())
        {
            BusinessEvent & business_event = *iter;
            ++iter;

            bool good = false;

            TcpConnection * connection = business_event.connection;

            switch (business_event.event)
            {
                case connect_notify:
                {
                    good = handle_connect(connection, connection->get_identity());
                    break;
                }
                case accept_notify:
                {
                    good = handle_accept(connection, connection->get_listener_port());
                    break;
                }
                case recv_notify:
                {
                    good = handle_recv(connection);
                    break;
                }
                case send_notify:
                {
                    good = handle_send(connection);
                    break;
                }
                case close_notify:
                {
                    handle_close(connection);
                    good = true;
                    break;
                }
                default:
                {
                    good = false;
                    break;
                }
            }

            if (!good)
            {
                do_close(connection);
            }

            connection->decrease_reference();

            if (!connection->has_reference())
            {
                /* when get to here, maybe connection->has_reference() is true */
                remove_connection(connection);
            }
        }

        business_event_list.clear();
    }

    RUN_LOG_DBG("proactor business thread-%d end", thread_index);
}

NAMESPACE_STUPID_NET_END


#endif // defined(_MSC_VER) && !defined(XACTOR_USE_SELECT)
