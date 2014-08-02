/********************************************************
 * Description : network switch (assistant of net module)
 * Data        : 2013-11-02 02:01:52
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifdef _MSC_VER
    #include <winsock2.h>
#else
    #include <signal.h>
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
        return(SIG_ERR);
    }
    else
    {
        return(old_act.sa_handler);
    }
}
#endif // _MSC_VER

NAMESPACE_STUPID_NET_BEGIN

NetSwitch::NetSwitch()
{
#ifdef _MSC_VER
    WSADATA wsa_data;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
    {
        RUN_LOG_CRI("WSAStartup failed: %d", stupid_net_error());
        exit(1);
    }
#else
    if (SIG_ERR == safe_signal(SIGPIPE, SIG_IGN))
    {
        RUN_LOG_CRI("safe_signal failed: %d", stupid_net_error());
        exit(1);
    }
#endif // _MSC_VER
}

NetSwitch::~NetSwitch()
{
#ifdef _MSC_VER
    if (0 != WSACleanup())
    {
        RUN_LOG_CRI("WSACleanup failed: %d", stupid_net_error());
        exit(1);
    }
#else
    if (SIG_ERR == safe_signal(SIGPIPE, SIG_DFL))
    {
        RUN_LOG_CRI("safe_signal failed: %d", stupid_net_error());
        exit(1);
    }
#endif // _MSC_VER
}

void NetSwitch::work()
{

}

NAMESPACE_STUPID_NET_END
