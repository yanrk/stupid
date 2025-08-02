#include "test_net.h"
#include "net/common/common.h"
#include "net/utility/tcp.h"
#include "net/utility/net_switch.h"
#include "base/utility/singleton.h"
#include "base/utility/log_switch.h"
#include "base/thread/thread.h"
#include "base/locker/locker.h"
#include "base/time/time.h"
#include <cstring>

USING_NAMESPACE_STUPID_BASE
USING_NAMESPACE_STUPID_NET

static Thread         s_server_thread;
static Thread         s_client_thread;
static ThreadLocker   s_stdout_locker;

static std::string    s_host_name("127.0.0.1");
static std::string    s_server_listen_port("3344");
static unsigned short s_client_bind_port = 4455;

static const char     s_test_data[] = "12345678901234567890123456789012";
static const size_t   s_test_data_size = sizeof(s_test_data) - 1;

static void handle_data(char * data, size_t data_len)
{
    for (size_t index = 0; index < data_len / 2; ++index)
    {
        char temp = data[index];
        data[index] = data[data_len - 1 - index];
        data[data_len - 1 - index] = temp;
    }
}

static thread_return_t STUPID_STDCALL server_thread_run_1(thread_argument_t)
{
    s_stdout_locker.acquire();
    printf("server thread start...\n");
    s_stdout_locker.release();

    socket_t listener = BAD_SOCKET;
    if (!tcp_listen(s_host_name.c_str(), s_server_listen_port.c_str(), listener))
    {
        assert(false);
    }

    socket_t accepter = BAD_SOCKET;
    if (!tcp_accept(listener, accepter))
    {
        assert(false);
    }

    tcp_close(listener);

    char buffer[s_test_data_size + 1] = { 0 };

    while (s_server_thread.running())
    {
        if (!tcp_recv_complete(accepter, buffer, 1))
        {
            break;
        }

        size_t data_len = buffer[0];
        if (!tcp_recv_complete(accepter, buffer + 1, data_len))
        {
            break;
        }

        handle_data(buffer + 1, data_len);

        if (!tcp_send_complete(accepter, buffer, 1 + data_len))
        {
            break;
        }
    }

    tcp_close(accepter);

    s_stdout_locker.acquire();
    printf("server thread end\n");
    s_stdout_locker.release();

    return THREAD_DEFAULT_RET;
}

static thread_return_t STUPID_STDCALL client_thread_run_1(thread_argument_t)
{
    s_stdout_locker.acquire();
    printf("client thread start...\n");
    s_stdout_locker.release();

    socket_t connecter = BAD_SOCKET;
    if (!tcp_connect(s_host_name.c_str(), s_server_listen_port.c_str(), connecter, "0.0.0.0", s_client_bind_port))
    {
        assert(false);
    }

    srand(static_cast<unsigned int>(stupid_time()));

    char buffer[s_test_data_size + 1] = { 0 };

    while (s_client_thread.running())
    {
        char data_len = static_cast<char>((rand() % (s_test_data_size - 5)) + 5 + 1);
        if (!tcp_send_complete(connecter, &data_len, 1))
        {
            break;
        }

        memcpy(buffer, s_test_data, data_len);
        if (!tcp_send_complete(connecter, buffer, data_len))
        {
            break;
        }

        buffer[static_cast<size_t>(data_len)] = '\0';
        s_stdout_locker.acquire();
        printf("client thread send data: %s\n", buffer);
        s_stdout_locker.release();

        if (!tcp_recv_complete(connecter, buffer, 1))
        {
            break;
        }

        data_len = buffer[0];
        if (!tcp_recv_complete(connecter, buffer, data_len))
        {
            break;
        }

        buffer[static_cast<size_t>(data_len)] = '\0';
        s_stdout_locker.acquire();
        printf("client thread recv data: %s\n", buffer);
        s_stdout_locker.release();
    }

    tcp_close(connecter);

    s_stdout_locker.acquire();
    printf("client thread end\n");
    s_stdout_locker.release();

    return THREAD_DEFAULT_RET;
}

static thread_return_t STUPID_STDCALL server_thread_run_2(thread_argument_t)
{
    s_stdout_locker.acquire();
    printf("server thread start...\n");
    s_stdout_locker.release();

    socket_t listener = BAD_SOCKET;
    if (!tcp_listen(s_host_name.c_str(), s_server_listen_port.c_str(), listener))
    {
        assert(false);
    }

    socket_t accepter = BAD_SOCKET;
    if (!tcp_accept(listener, accepter))
    {
        assert(false);
    }

    tcp_close(listener);

    while (s_server_thread.running())
    {
        if (!tcp_set_recv_buffer_size(accepter))
        {
            break;
        }

        if (!tcp_set_block_switch(accepter, false))
        {
            break;
        }

        if (!tcp_set_recv_timeout(accepter, 5000))
        {
            break;
        }

        char buffer[256] = { 0 };
        size_t need_len = 0;
        bool recv_ret = true;

        need_len = 1;
        recv_ret = tcp_recv_complete(accepter, buffer, need_len);
        assert(recv_ret);

        need_len = static_cast<size_t>(buffer[0] & 0x00FF);
        recv_ret = tcp_recv_complete(accepter, buffer, need_len);
        assert(recv_ret);

        s_stdout_locker.acquire();
        printf("server thread is done\n");
        s_stdout_locker.release();

        break;
    }

    tcp_close(accepter);

    s_stdout_locker.acquire();
    printf("server thread end\n");
    s_stdout_locker.release();

    return THREAD_DEFAULT_RET;
}

static thread_return_t STUPID_STDCALL client_thread_run_2(thread_argument_t)
{
    s_stdout_locker.acquire();
    printf("client thread start...\n");
    s_stdout_locker.release();

    socket_t connecter = BAD_SOCKET;
    if (!tcp_connect(s_host_name.c_str(), s_server_listen_port.c_str(), connecter))
    {
        assert(false);
    };

    while (s_client_thread.running())
    {
        if (!tcp_set_send_buffer_size(connecter))
        {
            break;
        }

        if (!tcp_set_block_switch(connecter, false))
        {
            break;
        }

        if (!tcp_set_send_timeout(connecter, 2000))
        {
            break;
        }

        char data[256] = { 0 };
        size_t data_len = 0;
        bool send_ret = true;

        data_len = 235;
        data[0] = static_cast<char>(data_len);
        send_ret = tcp_send_complete(connecter, data, data_len / 2);
        assert(send_ret);
        stupid_ms_sleep(1000);

        send_ret = tcp_send_complete(connecter, data + data_len / 2, data_len - data_len / 2 + 1);
        assert(send_ret);
        stupid_ms_sleep(1000);

        s_stdout_locker.acquire();
        printf("client thread is done\n");
        s_stdout_locker.release();

        break;
    }

    tcp_close(connecter);

    s_stdout_locker.acquire();
    printf("client thread end\n");
    s_stdout_locker.release();

    return THREAD_DEFAULT_RET;
}

static void test_net_tcp_1(void)
{
    Singleton<LogSwitch>::instance().init("./log.ini");
    Singleton<NetSwitch>::instance().init();

    s_server_thread.set_thread_args(server_thread_run_1, nullptr, "");
    s_server_thread.acquire();

    stupid_ms_sleep(1000);

    s_client_thread.set_thread_args(client_thread_run_1, nullptr, "");
    s_client_thread.acquire();

    stupid_ms_sleep(50);

    s_server_thread.release();

    stupid_ms_sleep(1000);

    s_client_thread.release();

    Singleton<NetSwitch>::instance().exit();
}

static void test_net_tcp_2(void)
{
    Singleton<LogSwitch>::instance().init("./log.ini");
    Singleton<NetSwitch>::instance().init();

    s_server_thread.set_thread_args(server_thread_run_2, nullptr, "");
    s_server_thread.acquire();

    stupid_ms_sleep(1000);

    s_client_thread.set_thread_args(client_thread_run_2, nullptr, "");
    s_client_thread.acquire();

    stupid_ms_sleep(1000);

    s_server_thread.release();
    s_client_thread.release();

    Singleton<NetSwitch>::instance().exit();
}

void test_net_tcp(void)
{
    test_net_tcp_1();
    printf("\n\n\n");
    test_net_tcp_2();
}
