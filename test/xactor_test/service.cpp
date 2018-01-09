#include <cassert>
#include <cstring>
#include "service.h"
#include "net/xactor/connection_base.h"
#include "base/log/log.h"
#include "base/common/memory.h"
#include "base/utility/guard.h"

static const char MSG_BLK[] = "this is a message.";
static const size_t MSG_LEN = sizeof(MSG_BLK) / sizeof(MSG_BLK[0]) - 1;

TestService::TestService(bool requester, bool test_data, size_t send_times)
    : m_requester(requester)
    , m_test_data(test_data)
    , m_max_msg_cnt(send_times)
    , m_connections()
    , m_locker()
{

}

TestService::~TestService()
{
    m_connections.clear();
}

bool TestService::on_connect(Stupid::Net::TcpConnectionBase * connection, size_t identity)
{
    assert(m_requester);

    if (m_requester)
    {
        RUN_LOG_DBG("connect client: %u", identity);
        return(insert_connection(connection) && send_message(connection));
    }
    else
    {
        RUN_LOG_CRI("on_connect handle exception");
        assert(false);
        return(false);
    }
}

bool TestService::on_accept(Stupid::Net::TcpConnectionBase * connection, unsigned short listener_port)
{
    assert(!m_requester);

    if (!m_requester)
    {
        RUN_LOG_DBG("accept from port: %d", listener_port);
        return(insert_connection(connection));
    }
    else
    {
        RUN_LOG_CRI("on_accept handle exception");
        assert(false);
        return(false);
    }
}

bool TestService::on_recv(Stupid::Net::TcpConnectionBase * connection)
{
    return(recv_message(connection));
}

bool TestService::on_send(Stupid::Net::TcpConnectionBase * connection)
{
    return(true);
}

bool TestService::on_close(Stupid::Net::TcpConnectionBase * connection)
{
    return(remove_connection(connection));
}

bool TestService::insert_connection(Stupid::Net::TcpConnectionBase * connection)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_connections.end() == m_connections.find(connection))
    {
        m_connections[connection] = 0;
        return(true);
    }
    else
    {
        RUN_LOG_CRI("insert repeated connection");
        assert(false);
        return(false);
    }
}

bool TestService::remove_connection(Stupid::Net::TcpConnectionBase * connection)
{
    Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
    if (m_connections.end() != m_connections.find(connection))
    {
        size_t count = m_connections[connection];
        if (m_test_data && count < m_max_msg_cnt)
        {
            RUN_LOG_CRI("connection %x not complete");
        }
        m_connections.erase(connection);
        return(true);
    }
    else
    {
        RUN_LOG_CRI("remove unknown connection");
        assert(false);
        return(false);
    }
}

bool TestService::send_message(Stupid::Net::TcpConnectionBase * connection)
{
    if (!m_test_data)
    {
        return(true);
    }

    char * data = nullptr;
    size_t data_len = 0;
    if (!generic_message(connection, data, data_len))
    {
        return(false);
    }

    RUN_LOG_TRK("connection %x send %06u bytes", connection, data_len);

    if (!connection->send_buffer_fill_len(data, data_len))
    {
        RUN_LOG_CRI("send_buffer_fill_len error");
        assert(false);
        return(false);
    }

    STUPID_DEL_ARR(data);

    return(true);
}

bool TestService::recv_message(Stupid::Net::TcpConnectionBase * connection)
{
    assert(m_test_data);

    if (connection->recv_buffer_size() < 2)
    {
        return(true);
    }

    char temp[2] = { 0x00 };
    if (!connection->recv_buffer_copy_len(temp, 2))
    {
        RUN_LOG_CRI("recv_buffer_copy_len error");
        assert(false);
        return(false);
    }

    size_t data_len = static_cast<unsigned char>(temp[0]) * 256U + static_cast<unsigned char>(temp[1]);
    if (connection->recv_buffer_size() < data_len)
    {
        connection->recv_buffer_water_mark(data_len);
        return(true);
    }

    char * data = nullptr;
    STUPID_NEW_ARR(data, char, data_len);
    if (nullptr == data)
    {
        return(false);
    }

    if (!connection->recv_buffer_move_len(data, data_len))
    {
        RUN_LOG_CRI("recv_buffer_move_len error");
        STUPID_DEL_ARR(data);
        assert(false);
        return(false);
    }

    if (!check_message(connection, data, data_len))
    {
        STUPID_DEL_ARR(data);
        assert(false);
        return(false);
    }

    STUPID_DEL_ARR(data);

    RUN_LOG_TRK("connection %x recv %06u bytes", connection, data_len);

    connection->recv_buffer_water_mark(2);

    return(send_message(connection));
}

bool TestService::generic_message(Stupid::Net::TcpConnectionBase * connection, char *& data, size_t & data_len)
{
    size_t count = 0;

    {
        Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
        count = m_connections[connection];
        if (count >= m_max_msg_cnt)
        {
            DBG_LOG("connection %x send all", connection);
            return(false);
        }
    }

    count = count % m_max_msg_cnt + 1;

    size_t memory_len = 2 + MSG_LEN * count + 1;
    char * memory_buf = nullptr;
    STUPID_NEW_ARR(memory_buf, char, memory_len);
    if (nullptr == memory_buf)
    {
        RUN_LOG_ERR("new memory buffer (%u) failed on connection %x", memory_len, connection);
        return(false);
    }

    data = memory_buf;
    data_len = 0;

    memory_buf[0] = static_cast<char>(memory_len / 256U);
    memory_buf[1] = static_cast<char>(memory_len % 256U);
    data_len += 2;
    for (size_t index = 0; index < count; ++index)
    {
        memcpy(memory_buf + data_len, MSG_BLK, MSG_LEN);
        data_len += MSG_LEN;
    }
    memory_buf[data_len] = '\0';
    data_len += 1;

    {
        Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
        m_connections[connection] = count;
    }

    return(true);
}

bool TestService::check_message(Stupid::Net::TcpConnectionBase * connection, const char * data, size_t data_len)
{
    size_t count = 0;

    {
        Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
        count = m_connections[connection];
        if (count >= m_max_msg_cnt)
        {
            RUN_LOG_CRI("connection %x send all", connection);
            return(false);
        }
    }

    count = count % m_max_msg_cnt + 1;

    const size_t need_len = 2 + MSG_LEN * count + 1;
    if (need_len != data_len)
    {
        RUN_LOG_ERR("connection %x message length error: need_len %u, recv_len %u", connection, need_len, data_len);
        return(false);
    }

    data += 2;
    data_len -= 2;
    for (size_t index = 0; index < count; ++index)
    {
        if (0 != memcmp(data, MSG_BLK, MSG_LEN))
        {
            RUN_LOG_ERR("connection %x message content error: %s", connection, data);
        }
        data += MSG_LEN;
        data_len -= MSG_LEN;
    }
    if ('\0' != data[0] || 1 != data_len)
    {
        RUN_LOG_ERR("connection %x message content error: %s", connection, data);
    }

    {
        Stupid::Base::Guard<Stupid::Base::ThreadLocker> guard(m_locker);
        m_connections[connection] = count;
    }

    return(true);
}
