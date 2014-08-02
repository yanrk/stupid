/********************************************************
 * Description : ftp helper
 * Data        : 2014-05-18 14:21:52
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_TOOL_FTP_H
#define STUPID_TOOL_FTP_H


#include <string>
#include "tool/common/common.h"

NAMESPACE_STUPID_TOOL_BEGIN

struct FtpInfo
{
    bool          m_verbose;
    bool          m_auth_peer;
    bool          m_auth_host;
    std::string   m_ftp_url;
    std::string   m_login_username;
    std::string   m_login_password;
    std::string   m_ca_file;            // 根证书，如："E:/ca_files/ca.crt"；如果用相对路径，此时路径必须加"./"开头
    std::string   m_key_password;
    std::string   m_host_key_file;      // 
    std::string   m_host_key_type;      // "PEM","DER","ENG"; but maybe not do well with "DER"
    std::string   m_host_crt_file;      // 
    std::string   m_host_crt_type;      // "PEM","DER","ENG"; but maybe not do well with "DER"
    std::string   m_host_root_path;
    std::string   m_peer_root_path;
};

class STUPID_API FtpHelper
{
public:
    static bool download(const FtpInfo & ftp_info);
};

NAMESPACE_STUPID_TOOL_END


#endif // STUPID_TOOL_FTP_H
