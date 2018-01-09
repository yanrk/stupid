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

#include <cstring>
#include <cstdlib>
#include <sstream>
#include "curl/curl.h"
#include "base/log/log.h"
#include "tool/utility/curl_switch.h"
#include "tool/mail/mail.h"

NAMESPACE_STUPID_TOOL_BEGIN

static void construct_mail_url(const MailInfo & mail_info, std::string & mail_url)
{
    mail_url.clear();

    std::stringstream oss;
    oss << "smtp://"
        << mail_info.m_smtp_host
        << ":"
        << mail_info.m_smtp_port;

    mail_url = oss.str();
}

static std::string construct_mail_receivers(const std::list<std::string> & receiver_list)
{
    std::string receivers;

    std::list<std::string>::const_iterator iter_b = receiver_list.begin();
    std::list<std::string>::const_iterator iter_e = receiver_list.end();
    std::list<std::string>::const_iterator iter_f;
    for (iter_f = iter_b; iter_f != iter_e; ++iter_f)
    {
        if (iter_f != iter_b)
        {
            receivers += ",";
        }
        receivers += iter_f->c_str();
    }

    return(receivers);
}

static void construct_mail_data(const MailInfo & mail_info, std::string & mail_data)
{
    mail_data.clear();

    std::string mail_version("MIME-Version: 1.0\r\n");

    std::string mail_from;
    if (!mail_info.m_mail_from.empty())
    {
        mail_from += "From: ";
        mail_from += mail_info.m_nickname;
        mail_from += mail_info.m_mail_from;
        mail_from += "\r\n";
    }

    std::string mail_to;
    if (!mail_info.m_mail_to_list.empty())
    {
        mail_to += "To: ";
        mail_to += construct_mail_receivers(mail_info.m_mail_to_list);
        mail_to += "\r\n";
    }

    std::string mail_cc;
    if (!mail_info.m_mail_cc_list.empty())
    {
        mail_cc += "CC: ";
        mail_cc += construct_mail_receivers(mail_info.m_mail_cc_list);
        mail_cc += "\r\n";
    }

    std::string mail_bcc;
    if (!mail_info.m_mail_bcc_list.empty())
    {
        mail_bcc += "BCC: ";
        mail_bcc += construct_mail_receivers(mail_info.m_mail_bcc_list);
        mail_bcc += "\r\n";
    }

    std::string mail_subject("Subject: ");
    mail_subject += (mail_info.m_mail_subject.empty() ? "unknown" : mail_info.m_mail_subject);
    mail_subject += "\r\n";

    std::string mail_encoding("Content-Transfer-Encoding: 8bit\r\n");

    std::string mail_type("Content-Type: text/html;\r\n");

    std::string mail_charset("Charset=\"UTF-8\"\r\n");

    std::string mail_header_ending("\r\n");

    mail_data.reserve(
        mail_version.size() + 
        mail_from.size() + 
        mail_to.size() + 
        mail_cc.size() + 
        mail_bcc.size() + 
        mail_subject.size() + 
        mail_encoding.size() + 
        mail_type.size() + 
        mail_charset.size() + 
        mail_header_ending.size() + 
        mail_info.m_mail_content.size()
    );

    mail_data = mail_version
              + mail_from
              + mail_to
              + mail_cc
              + mail_bcc
              + mail_subject
              + mail_encoding
              + mail_type
              + mail_charset
              + mail_header_ending
              + mail_info.m_mail_content;
}

static void construct_mail_rcpt(const MailInfo & mail_info, std::list<std::string> & mail_rcpt)
{
    mail_rcpt.clear();
    mail_rcpt.insert(mail_rcpt.end(), mail_info.m_mail_to_list.begin(), mail_info.m_mail_to_list.end());
    mail_rcpt.insert(mail_rcpt.end(), mail_info.m_mail_cc_list.begin(), mail_info.m_mail_cc_list.end());
    mail_rcpt.insert(mail_rcpt.end(), mail_info.m_mail_bcc_list.begin(), mail_info.m_mail_bcc_list.end());
}

struct MailSendInfo
{
    const std::string & data;
    size_t              sent_len;
    MailSendInfo(const std::string & mail_data) : data(mail_data), sent_len(0) { }
};

static size_t mail_send_callback(void * ptr, size_t size, size_t nmemb, void * user_data)
{
    char * buff = reinterpret_cast<char *>(ptr);
    if (nullptr == buff)
    {
        RUN_LOG_ERR("mail_send_callback failed: buff is null");
        return(0);
    }

    size_t buff_size = size * nmemb;
    if (0 == buff_size)
    {
        RUN_LOG_ERR("mail_send_callback failed: buff size is 0");
        return(0);
    }

    MailSendInfo * mail_send_info = reinterpret_cast<MailSendInfo *>(user_data);
    if (nullptr == mail_send_info)
    {
        RUN_LOG_ERR("mail_send_callback failed: mail send info is null");
        return(0);
    }

    size_t send_len = mail_send_info->data.size() - mail_send_info->sent_len;
    if (send_len > buff_size)
    {
        send_len = buff_size;
    }

    if (0 != send_len)
    {
        memcpy(buff, mail_send_info->data.c_str() + mail_send_info->sent_len, send_len);
        mail_send_info->sent_len += send_len;
    }

    return(send_len);
}

bool MailHelper::send_mail(const MailInfo & mail_info)
{
    Stupid::Base::Singleton<CurlSwitch>::instance().work();

    std::string mail_url;
    std::string mail_data;
    std::list<std::string> mail_rcpt;
    construct_mail_url(mail_info, mail_url);
    construct_mail_data(mail_info, mail_data);
    construct_mail_rcpt(mail_info, mail_rcpt);
    MailSendInfo mail_send_info(mail_data);

    CURL * curl = nullptr;
    struct curl_slist * rcpt_list = nullptr;

    bool ret = false;
    do
    {
        curl = curl_easy_init();
        if (nullptr == curl)
        {
            RUN_LOG_ERR("curl_easy_init failed");
            break;
        }

        std::list<std::string>::const_iterator iter_b = mail_rcpt.begin();
        std::list<std::string>::const_iterator iter_e = mail_rcpt.end();
        std::list<std::string>::const_iterator iter_f;
        for (iter_f = iter_b; iter_f != iter_e; ++iter_f)
        {
            rcpt_list = curl_slist_append(rcpt_list, iter_f->c_str());
        }
        if (nullptr == rcpt_list)
        {
            RUN_LOG_ERR("rcpt list is null");
            break;
        }

        curl_easy_setopt(curl, CURLOPT_VERBOSE, mail_info.m_verbose ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_SESSIONID_CACHE, 0L);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY); /* CURLUSESSL_ALL */
        curl_easy_setopt(curl, CURLOPT_URL, mail_url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, mail_info.m_username.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, mail_info.m_password.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, mail_info.m_mail_from.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, rcpt_list);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, mail_send_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &mail_send_info);

        CURLcode curl_code = curl_easy_perform(curl);
        if (CURLE_OK != curl_code)
        {
            const char * curl_error = curl_easy_strerror(curl_code);
            RUN_LOG_ERR("curl_easy_perform failed(%d): %s", curl_code, (nullptr == curl_error ? "unknown" : curl_error));
            break;
        }

        ret = true;
    } while (false);

    if (nullptr != rcpt_list)
    {
        curl_slist_free_all(rcpt_list);
    }
    if (nullptr != curl)
    {
        curl_easy_cleanup(curl);
    }

    return(ret);
}

NAMESPACE_STUPID_TOOL_END
