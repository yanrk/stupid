/********************************************************
 * Description : linux tcp connection reactor class
 * Data        : 2014-07-01 18:07:20
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_NET_REACTOR_H
#define STUPID_NET_REACTOR_H


#ifndef _MSC_VER


#include <arpa/inet.h>
#include <sys/epoll.h>
#include <set>
#include <list>
#include <vector>
#include "net/common/common.h"
#include "net/xactor/block.h"
#include "net/xactor/connection.h"
#include "base/thread/thread.h"
#include "base/locker/locker.h"
#include "base/pool/object_pool.h"
#include "base/utility/uncopy.h"
#include "base/utility/id_manager.h"

NAMESPACE_STUPID_NET_BEGIN

class TcpManager;

class TcpReactor : private Stupid::Base::Uncopy
{
private:
    struct DataEvents
    {
        TcpConnection * connection;
        size_t          events;
    };

    struct BusinessEvent
    {
        TcpConnection * connection;
        size_t          event;
    };

private:
    typedef TcpConnection::BlockPool                                      BlockPool;
    typedef std::list<DataEvents>                                         DataEventsList;
    typedef std::vector<DataEventsList>                                   DataEventsListVector;
    typedef std::list<BusinessEvent>                                      BusinessEventList;
    typedef std::vector<BusinessEventList>                                BusinessEventListVector;
    typedef std::set<TcpConnection *>                                     ConnectionSet;
    typedef Stupid::Base::IDManager<size_t, Stupid::Base::NullLocker>     UNIQUE_CREATOR;
    typedef std::vector<TcpConnection *>                                  ConnectionVector;

public:
    TcpReactor();
    ~TcpReactor();

public:
    bool init(TcpManager * manager, size_t event_thread_count, size_t handle_thread_count, unsigned short * service_port, size_t service_port_count);
    void exit();

public:
    void reactor_connection_process();
    void reactor_data_process(size_t thread_index);
    void reactor_business_process(size_t thread_index);

public:
    bool create_connection(const sockaddr_in_t & server_address, size_t identity, const char * bind_ip, unsigned short bind_port);
    void connection_send(TcpConnection * connection);
    void close_connection(TcpConnection * connection);

private:
    bool running();

private:
    bool create_listener(unsigned short * service_port, size_t service_port_count);
    void destroy_listener();

private:
    bool create_epoll();
    void destroy_epoll();
    bool append_connection_to_epoll(TcpConnection * connection);
    bool delete_connection_from_epoll(TcpConnection * connection);
    bool modify_connection_of_epoll(TcpConnection * connection, bool send, bool recv);

private:
    bool do_connect(const sockaddr_in_t & server_address, size_t identity, const char * bind_ip, unsigned short bind_port);
    bool do_accept(TcpConnection * listener_connection);
    bool do_recv(TcpConnection * connection);
    bool do_send(TcpConnection * connection);
    void do_close(TcpConnection * connection);

private:
    bool handle_connect(TcpConnection * connection, size_t identity);
    bool handle_accept(TcpConnection * connection, unsigned short listener_port);
    bool handle_recv(TcpConnection * connection);
    bool handle_send(TcpConnection * connection);
    bool handle_close(TcpConnection * connection);

private:
    bool acquire_reactor_threads(size_t event_thread_count, size_t handle_thread_count);
    void release_reactor_threads();

private:
    TcpConnection * acquire_connection();
    void release_connection(TcpConnection *& connection);

private:
    void insert_connection(TcpConnection * connection);
    void remove_connection(TcpConnection *& connection);
    void destroy_invalid_connections();
    void destroy_normal_connections();
    void destroy_closed_connections();
    void destroy_binded_connections();
    void destroy_connections();

private:
    void append_data_events(DataEventsListVector & data_events_list_vector);
    void delete_data_events(size_t thread_index, DataEventsList & events_list);
    void clear_data_events();

private:
    void append_business_event(BusinessEvent & business_event);
    void delete_business_event(size_t thread_index, BusinessEventList & event_list);
    void clear_business_event();

private:
    volatile bool                                  m_running;
    TcpManager                                   * m_manager;
    int                                            m_epoll;
    ConnectionVector                               m_listeners;
    Stupid::Base::Thread                         * m_thread;
    size_t                                         m_thread_count;
    BlockPool                                      m_block_pool;
    DataEventsListVector                           m_data_events_list_vector;
    Stupid::Base::ThreadLocker                     m_data_events_list_vector_locker;
    BusinessEventListVector                        m_business_event_list_vector;
    Stupid::Base::ThreadLocker                     m_business_event_list_vector_locker;
    ConnectionSet                                  m_normal_connection_set;
    ConnectionSet                                  m_closed_connection_set;
    Stupid::Base::ThreadLocker                     m_connection_set_locker;
    UNIQUE_CREATOR                                 m_unique_creator;
    ConnectionSet                                  m_binded_connection_set;
    Stupid::Base::ThreadLocker                     m_binded_connection_set_locker;
};

NAMESPACE_STUPID_NET_END


#endif // _MSC_VER


#endif // STUPID_NET_REACTOR_H
