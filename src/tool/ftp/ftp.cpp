/********************************************************
 * Description : ftp helper
 * Data        : 2014-05-18 14:21:52
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <list>
#include <string>
#include <fstream>
#include "curl/curl.h"
#include "base/filesystem/sys_io.h"
#include "base/filesystem/directory.h"
#include "base/log/log.h"
#include "tool/utility/curl_switch.h"
#include "tool/ftp/ftp.h"

NAMESPACE_STUPID_TOOL_BEGIN

static void ftp_set_common_option(
                CURL * curl, 
                const FtpInfo & ftp_info, 
                const std::string & transfer_url
            )
{
    curl_easy_setopt(curl, CURLOPT_VERBOSE, ftp_info.m_verbose ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_URL, transfer_url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERNAME, ftp_info.m_login_username.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, ftp_info.m_login_password.c_str());

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    if (!ftp_info.m_auth_peer && !ftp_info.m_auth_host)
    {
        return;
    }

    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_FTP_SSL_CCC, CURLFTPSSL_CCC_NONE);
    curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);

    if (ftp_info.m_auth_peer)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 2L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, ftp_info.m_ca_file.c_str());
    }

    if (ftp_info.m_auth_host)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(curl, CURLOPT_SSLCERT, ftp_info.m_host_crt_file.c_str());
        curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, ftp_info.m_host_crt_type.c_str());
        curl_easy_setopt(curl, CURLOPT_SSLKEY, ftp_info.m_host_key_file.c_str());
        curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, ftp_info.m_host_key_type.c_str());
        if (!ftp_info.m_key_password.empty())
        {
            curl_easy_setopt(curl, CURLOPT_KEYPASSWD, ftp_info.m_key_password.c_str());
        }
    }
}

static bool ftp_remote_path_is_directory(
                const FtpInfo & ftp_info
            )
{
    CURL * curl = curl_easy_init();
    if (nullptr == curl)
    {
        RUN_LOG_ERR("curl_easy_init failed");
        return(false);
    }

    std::string remote_path = ftp_info.m_ftp_url + "/" + ftp_info.m_peer_root_path + "/";
    ftp_set_common_option(curl, ftp_info, remote_path);

    curl_easy_setopt(curl, CURLOPT_DIRLISTONLY, 1L);

    CURLcode curl_code = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return(CURLE_OK == curl_code);
}

struct FtpRecvInfo
{
    std::string              remot_dir_name;
    std::string              local_dir_name;
    std::ofstream            local_current_ofs;
    std::list<std::string>   remot_sub_dir_list;
};

static long ftp_recv_chunk_begin_callback(
                struct curl_fileinfo * remote_info, 
                struct FtpRecvInfo * ftp_recv_info, 
                int /* remains */
            )
{
    if (CURLFILETYPE_DIRECTORY == remote_info->filetype)
    {
        ftp_recv_info->remot_sub_dir_list.push_back(ftp_recv_info->remot_dir_name + "/" + remote_info->filename);
        return(CURL_CHUNK_BGN_FUNC_SKIP);
    }
    else
    {
        if (ftp_recv_info->local_current_ofs.is_open())
        {
            ftp_recv_info->local_current_ofs.close();
        }
        std::string file_name(ftp_recv_info->local_dir_name + "/" + remote_info->filename);
        stupid_rmdir(file_name.c_str());
        ftp_recv_info->local_current_ofs.open(file_name.c_str(), std::ios::binary | std::ios::trunc);
        if (ftp_recv_info->local_current_ofs.is_open())
        {
            return(CURL_CHUNK_BGN_FUNC_OK);
        }
        else
        {
            RUN_LOG_ERR("create %s failed", file_name.c_str());
            return(CURL_CHUNK_BGN_FUNC_FAIL);
        }
    }
}

static long ftp_recv_chunk_end_callback(
                struct FtpRecvInfo * ftp_recv_info
            )
{
    if (ftp_recv_info->local_current_ofs.is_open())
    {
        ftp_recv_info->local_current_ofs.close();
    }
    return(CURL_CHUNK_END_FUNC_OK);
}

static size_t ftp_recv_data_callback(
                  void * ptr, size_t size, size_t nmemb, void * user_data
              )
{
    const size_t recv_len = size * nmemb;
    const char * data = reinterpret_cast<char *>(ptr);
    struct FtpRecvInfo * ftp_recv_info = reinterpret_cast<struct FtpRecvInfo *>(user_data);
    ftp_recv_info->local_current_ofs.write(data, recv_len);
    return(recv_len);
}

static bool ftp_download_remote_directory(
                const FtpInfo & ftp_info, 
                std::list<std::string> & remot_dir_list
            )
{
    if (remot_dir_list.empty())
    {
        DBG_LOG("remote directory list is empty");
        return(false);
    }

    CURL * curl = curl_easy_init();
    if (nullptr == curl)
    {
        RUN_LOG_ERR("curl_easy_init failed");
        return(false);
    }

    std::string remot_dir_name = remot_dir_list.front();
    remot_dir_list.pop_front();

    std::string local_dir_name(ftp_info.m_host_root_path + "/" + remot_dir_name);
    Stupid::Base::stupid_create_directory_recursive(local_dir_name);

    struct FtpRecvInfo ftp_recv_info;
    ftp_recv_info.local_dir_name = local_dir_name;
    ftp_recv_info.remot_dir_name = remot_dir_name;

    std::string download_url(ftp_info.m_ftp_url + "/" + ftp_recv_info.remot_dir_name + "/*");
    ftp_set_common_option(curl, ftp_info, download_url);

    curl_easy_setopt(curl, CURLOPT_WILDCARDMATCH, 1L);
    curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, ftp_recv_chunk_begin_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, ftp_recv_chunk_end_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_DATA, &ftp_recv_info);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_recv_data_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftp_recv_info);

    bool ret = true;

    CURLcode curl_code = curl_easy_perform(curl);
    if (CURLE_OK != curl_code)
    {
        ret = false;
        const char * curl_error = curl_easy_strerror(curl_code);
        std::string remote_path = ftp_info.m_ftp_url + "/" + ftp_recv_info.remot_dir_name;
        RUN_LOG_ERR("curl_easy_perform() failed: %s, when download from %s", (nullptr == curl_error ? "unknown" : curl_error), remote_path.c_str());
    }
    curl_easy_cleanup(curl);

    remot_dir_list.splice(remot_dir_list.end(), ftp_recv_info.remot_sub_dir_list);

    return(ret);
}

static bool ftp_download_remote_file(
                const FtpInfo & ftp_info
            )
{
    std::string file_name(ftp_info.m_host_root_path + "/" + ftp_info.m_peer_root_path);
    Stupid::Base::stupid_create_directory_recursive(file_name);
    stupid_rmdir(file_name.c_str());

    struct FtpRecvInfo ftp_recv_info;
    ftp_recv_info.local_current_ofs.open(file_name.c_str(), std::ios::binary | std::ios::trunc);
    if (!ftp_recv_info.local_current_ofs.is_open())
    {
        RUN_LOG_ERR("create %s failed", file_name.c_str());
        return(false);
    }

    CURL * curl = curl_easy_init();
    if (nullptr == curl)
    {
        RUN_LOG_ERR("curl_easy_init failed");
        return(false);
    }

    std::string download_url(ftp_info.m_ftp_url + "/" + ftp_info.m_peer_root_path);
    ftp_set_common_option(curl, ftp_info, download_url);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_recv_data_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftp_recv_info);

    bool ret = true;

    CURLcode curl_code = curl_easy_perform(curl);
    if (CURLE_OK != curl_code)
    {
        ret = false;
        ftp_recv_info.local_current_ofs.close();
        stupid_unlink(file_name.c_str());
        const char * curl_error = curl_easy_strerror(curl_code);
        RUN_LOG_ERR("curl_easy_perform() failed: %s, when download from %s", (nullptr == curl_error ? "unknown" : curl_error), download_url.c_str());
    }
    curl_easy_cleanup(curl);

    return(ret);
}

bool FtpHelper::download(const FtpInfo & ftp_info)
{
    Stupid::Base::Singleton<CurlSwitch>::instance().work();

    if (ftp_remote_path_is_directory(ftp_info))
    {
        std::list<std::string> remot_dir_list(1, ftp_info.m_peer_root_path);
        while (!remot_dir_list.empty())
        {
            ftp_download_remote_directory(ftp_info, remot_dir_list);
        }
        return(true);
    }
    else
    {
        return(ftp_download_remote_file(ftp_info));
    }
}

NAMESPACE_STUPID_TOOL_END
