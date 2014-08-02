#include "tool/ftp/ftp.h"

USING_NAMESPACE_STUPID_TOOL

void test_tool_ftp(void)
{
    bool test_auth_peer = false;

    FtpInfo ftp_info;
    ftp_info.m_verbose = true;
    ftp_info.m_auth_host = false;
    ftp_info.m_login_username = "username";
    ftp_info.m_login_password = "password";
    ftp_info.m_host_root_path = "./ftp_download/";
    ftp_info.m_peer_root_path = "logger";

    if (test_auth_peer)
    {
        ftp_info.m_auth_peer = true;
        ftp_info.m_ftp_url = "ftps://127.0.0.1:9999/";
        ftp_info.m_ca_file = "./certificate.crt";
    }
    else
    {
        ftp_info.m_auth_peer = false;
        ftp_info.m_ftp_url = "ftp://127.0.0.1/";
    }

    if (FtpHelper::download(ftp_info))
    {
        printf("ftp download ok\n");
    }
    else
    {
        printf("ftp download failed\n");
    }
}
