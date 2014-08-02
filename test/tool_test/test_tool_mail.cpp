#include "tool/mail/mail.h"

USING_NAMESPACE_STUPID_TOOL

void test_tool_mail(void)
{
    MailInfo mail;
    mail.m_verbose = true;
    mail.m_username = "test_curl";
    mail.m_password = "test_curl_123!";
    mail.m_nickname = "yanrk";
    mail.m_smtp_host = "smtp.126.com";
    mail.m_smtp_port = 25;
    mail.m_mail_from = "<test_curl@126.com>";
    mail.m_mail_to_list.push_back("<ken_scott@126.com>");
    mail.m_mail_cc_list.push_back("<ken_scott@163.com>");
    mail.m_mail_bcc_list.push_back("<feeling_dxl@yeah.net>");
    mail.m_mail_subject = "TEST MAIL";
    mail.m_mail_content = "<div>我自横刀向天笑，去留肝胆两昆仑</div>";
    if (MailHelper::send_mail(mail))
    {
        printf("send mail ok\n");
    }
    else
    {
        printf("send mail failed\n");
    }
}
