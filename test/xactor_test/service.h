#ifndef STUPID_TCP_XACTOR_TEST_SERVICE_H
#define STUPID_TCP_XACTOR_TEST_SERVICE_H


#include <map>
#include "net/xactor/service_base.h"
#include "base/locker/locker.h"

class TestService : public Stupid::Net::TcpServiceBase
{
public:
    TestService(bool requester, bool test_data, size_t send_times);
    virtual ~TestService();

private:
    virtual bool on_connect(Stupid::Net::TcpConnectionBase * connection, size_t identity);
    virtual bool on_accept(Stupid::Net::TcpConnectionBase * connection, unsigned short listener_port);
    virtual bool on_recv(Stupid::Net::TcpConnectionBase * connection);
    virtual bool on_send(Stupid::Net::TcpConnectionBase * connection);
    virtual bool on_close(Stupid::Net::TcpConnectionBase * connection);

private:
    bool insert_connection(Stupid::Net::TcpConnectionBase * connection);
    bool remove_connection(Stupid::Net::TcpConnectionBase * connection);

private:
    bool generic_message(Stupid::Net::TcpConnectionBase * connection, char *& data, size_t & data_len);
    bool check_message(Stupid::Net::TcpConnectionBase * connection, const char * data, size_t data_len);

private:
    bool send_message(Stupid::Net::TcpConnectionBase * connection);
    bool recv_message(Stupid::Net::TcpConnectionBase * connection);

private:
    bool                                                   m_requester;
    bool                                                   m_test_data;
    size_t                                                 m_max_msg_cnt;
    std::map<Stupid::Net::TcpConnectionBase *, size_t>     m_connections;
    Stupid::Base::ThreadLocker                             m_locker;
};


#endif // STUPID_TCP_XACTOR_TEST_SERVICE_H
