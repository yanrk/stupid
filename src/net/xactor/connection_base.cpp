/********************************************************
 * Description : connection base class
 * Data        : 2014-07-05 12:39:45
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include "net/xactor/connection_base.h"

NAMESPACE_STUPID_NET_BEGIN

TcpConnectionBase::TcpConnectionBase()
    : m_user_data(nullptr)
{

}

TcpConnectionBase::~TcpConnectionBase()
{

}

void TcpConnectionBase::set_user_data(void * user_data)
{
    m_user_data = user_data;
}

void * TcpConnectionBase::get_user_data()
{
    return(m_user_data);
}

NAMESPACE_STUPID_NET_END
