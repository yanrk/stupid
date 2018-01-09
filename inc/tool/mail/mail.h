/********************************************************
 * Description : mail helper
 * Data        : 2014-04-20 22:56:11
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_TOOL_MAIL_H
#define STUPID_TOOL_MAIL_H


#include <list>
#include <string>
#include "tool/common/common.h"

NAMESPACE_STUPID_TOOL_BEGIN

struct MailInfo
{
    bool                     m_verbose;
    std::string              m_username;
    std::string              m_password;
    std::string              m_nickname;
    std::string              m_smtp_host;
    int                      m_smtp_port;
    std::string              m_mail_from;
    std::string              m_mail_subject;
    std::string              m_mail_content;
    std::list<std::string>   m_mail_to_list;
    std::list<std::string>   m_mail_cc_list;
    std::list<std::string>   m_mail_bcc_list;
};

class STUPID_API MailHelper
{
public:
    static bool send_mail(const MailInfo & mail_info);
};

NAMESPACE_STUPID_TOOL_END


#endif // STUPID_TOOL_MAIL_H
