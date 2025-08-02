/********************************************************
 * Description : network switch (assistant of net module)
 * Data        : 2013-11-02 02:01:52
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER
    #include <winsock2.h>
#else
    #include <signal.h>
    #include <sys/resource.h>
#endif // _MSC_VER

#include <cstdlib>
#include "base/log/log.h"
#include "net/utility/net_switch.h"

#ifndef _MSC_VER
typedef void sig_func(int);

static sig_func * safe_signal(int signo, sig_func * func)
{
    struct sigaction new_act;
    new_act.sa_handler = func;
    sigemptyset(&new_act.sa_mask);
    new_act.sa_flags = 0;
    if (SIGALRM == signo)
    {
    #ifdef SA_INTERRUPT
        new_act.sa_flags |= SA_INTERRUPT;
    #endif
    }
    else
    {
    #ifdef SA_RESTART
        new_act.sa_flags |= SA_RESTART;
    #endif
    }

    struct sigaction old_act;
    if (sigaction(signo, &new_act, &old_act) < 0)
    {
        return SIG_ERR;
    }
    else
    {
        return old_act.sa_handler;
    }
}

static bool increase_socket_limit()
{
    struct rlimit old_limit;
    if (0 != getrlimit(RLIMIT_NOFILE, &old_limit))
    {
        return false;
    }

    struct rlimit new_limit;
    new_limit.rlim_cur = new_limit.rlim_max = RLIM_INFINITY;
    if (0 == setrlimit(RLIMIT_NOFILE, &new_limit))
    {
        return true;
    }

    if (old_limit.rlim_max < 65535)
    {
        new_limit.rlim_cur = new_limit.rlim_max = 65535;
        if (0 == setrlimit(RLIMIT_NOFILE, &new_limit))
        {
            return true;
        }
    }

    if (old_limit.rlim_cur == old_limit.rlim_max)
    {
        return true;
    }

    new_limit.rlim_cur = new_limit.rlim_max = old_limit.rlim_max;
    if (0 == setrlimit(RLIMIT_NOFILE, &new_limit))
    {
        return true;
    }

    return false;
}
#endif // _MSC_VER

NAMESPACE_STUPID_NET_BEGIN

NetSwitch::NetSwitch()
    : m_init(false)
{

}

NetSwitch::~NetSwitch()
{
    exit();
}

bool NetSwitch::init()
{
    if (m_init)
    {
        return true;
    }

#ifdef _MSC_VER
    WSADATA wsa_data;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
    {
        RUN_LOG_CRI("WSAStartup failed: %d", stupid_net_error());
        return false;
    }
#else
    if (SIG_ERR == safe_signal(SIGPIPE, SIG_IGN))
    {
        RUN_LOG_CRI("safe_signal failed: %d", stupid_net_error());
        return false;
    }
    increase_socket_limit();
#endif // _MSC_VER

    m_init = true;
    return true;
}

void NetSwitch::exit()
{
    if (!m_init)
    {
        return;
    }

#ifdef _MSC_VER
    if (0 != WSACleanup())
    {
        RUN_LOG_CRI("WSACleanup failed: %d", stupid_net_error());
    }
#else
    if (SIG_ERR == safe_signal(SIGPIPE, SIG_DFL))
    {
        RUN_LOG_CRI("safe_signal failed: %d", stupid_net_error());
    }
#endif // _MSC_VER

    m_init = false;
}

NAMESPACE_STUPID_NET_END
