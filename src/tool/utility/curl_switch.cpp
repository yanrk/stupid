/********************************************************
 * Description : curl switch (initialization of curl)
 * Data        : 2014-05-18 18:02:54
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <cstdlib>
#include "curl/curl.h"
#include "base/log/log.h"
#include "tool/utility/curl_switch.h"

NAMESPACE_STUPID_TOOL_BEGIN

CurlSwitch::CurlSwitch()
{
    CURLcode curl_code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (CURLE_OK != curl_code)
    {
        const char * curl_error = curl_easy_strerror(curl_code);
        RUN_LOG_ERR("curl_global_init failed(%d): %s",
                    curl_code, (nullptr == curl_error ? "unknown" : curl_error));
        exit(0);
    }
}

CurlSwitch::~CurlSwitch()
{
    curl_global_cleanup();
}

void CurlSwitch::work()
{

}

NAMESPACE_STUPID_TOOL_END
