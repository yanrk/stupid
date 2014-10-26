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

#ifndef STUPID_NET_NET_SWITCH_H
#define STUPID_NET_NET_SWITCH_H


#include "net/common/common.h"
#include "base/utility/uncopy.h"
#include "base/utility/singleton.h"

NAMESPACE_STUPID_NET_BEGIN

class STUPID_API NetSwitch : private Stupid::Base::Uncopy
{
public:
    bool init();
    void exit();

private:
    NetSwitch();
    ~NetSwitch();

private:
    friend class Stupid::Base::Singleton<NetSwitch>;

private:
    bool                   m_init;
};

NAMESPACE_STUPID_NET_END


#endif // STUPID_NET_NET_SWITCH_H
