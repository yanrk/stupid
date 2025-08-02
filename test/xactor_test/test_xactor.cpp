#include <cstdio>
#include <cstring>
#include <string>
#include "test_xactor.h"
#include "service.h"
#include "net/xactor/manager.h"
#include "net/utility/utility.h"
#include "net/utility/net_switch.h"
#include "base/log/log.h"
#include "base/time/time.h"
#include "base/config/ini.h"
#include "base/utility/singleton.h"
#include "base/utility/log_switch.h"

static bool             s_requester;
static bool             s_test_data;
static size_t           s_send_times;
static size_t           s_thread_count;
static std::string      s_host_name;
static unsigned short   s_host_port;
static size_t           s_connection_count;
static sockaddr_in_t    s_address;

static bool get_config()
{
    const char * const ini_file = "./service.ini";

    Stupid::Base::Ini ini;
    if (!ini.load(ini_file))
    {
        printf("load %s failed\n", ini_file);
        return false;
    }

    if (!ini.get_value("", "requester", s_requester))
    {
        printf("get value %s failed\n", "requester");
        return false;
    }

    if (!ini.get_value("", "test_data", s_test_data))
    {
        printf("get value %s failed\n", "test_data");
        return false;
    }

    if (!ini.get_value("", "send_times", s_send_times))
    {
        printf("get value %s failed\n", "send_times");
        return false;
    }

    if (!ini.get_value("", "thread_count", s_thread_count))
    {
        printf("get value %s failed\n", "thread_count");
        return false;
    }

    if (!ini.get_value("", "host_port", s_host_port))
    {
        printf("get value %s failed\n", "host_port");
        return false;
    }

    if (s_requester)
    {
        if (!ini.get_value("", "host_name", s_host_name))
        {
            printf("get value %s failed\n", "host_name");
            return false;
        }

        if (!ini.get_value("", "connection_count", s_connection_count))
        {
            printf("get value %s failed\n", "connection_count");
            return false;
        }

        if (!Stupid::Net::transform_address(s_host_name.c_str(), s_host_port, s_address))
        {
            printf("transform(%s : %d) to address failed\n", s_host_name.c_str(), s_host_port);
            return false;
        }
    }

    ini.clear();

    return true;
}

void test_tcp_xactor(void)
{
    if (!Stupid::Base::Singleton<Stupid::Base::LogSwitch>::instance().init("./log.ini"))
    {
        printf("load log config failed\n");
        return;
    }

    printf("log service init...\n");

    if (!Stupid::Base::Singleton<Stupid::Net::NetSwitch>::instance().init())
    {
        printf("net switch init failed\n");
        return;
    }

    printf("new switch init...\n");

    if (!get_config())
    {
        return;
    }

    TestService service(s_requester, s_test_data, s_send_times);

    Stupid::Net::TcpManager manager;

    manager.set_service(&service);

    if (s_requester)
    {
        if (!manager.init(0, s_thread_count, 0))
        {
            printf("tcp client tester init failed\n");
            return;
        }
    }
    else
    {
        if (!manager.init(0, s_thread_count, &s_host_port, 1))
        {
            printf("tcp server tester init failed\n");
            return;
        }
    }

    printf("tcp %s tester init...\n", (s_requester ? "client" : "server"));

    if (s_requester)
    {
        printf("\n");
        for (size_t index = 0; index < s_connection_count; ++index)
        {
            if (manager.create_connection(s_address, index))
            {
                printf("create connection %d success...\n", index + 1);
            }
            else
            {
                printf("create connection %d failed...\n", index + 1);
                break;
            }
        }
    }

    const int console_buffer_size = 4096;
    char buffer[console_buffer_size];
    while (true)
    {
        printf("\ninput \"stop\" / \"exit\" / \"quit\" to exit\n");
        if (1 != scanf("%s", buffer))
        {
            continue;
        }
        if (0 == strcmp("stop", buffer) ||
            0 == strcmp("exit", buffer) ||
            0 == strcmp("quit", buffer))
        {
            break;
        }
    }
    printf("\n");

    manager.exit();

    printf("tcp %s tester exit...\n", (s_requester ? "client" : "server"));

    Stupid::Base::Singleton<Stupid::Net::NetSwitch>::instance().exit();

    Stupid::Base::Singleton<Stupid::Base::LogSwitch>::instance().exit();

    printf("log service exit...\n");

    Stupid::Base::stupid_ms_sleep(2000);
}
