/********************************************************
 * Description : curl switch (initialization of curl)
 * Data        : 2014-05-18 18:02:54
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_TOOL_CURL_SWITCH_H
#define STUPID_TOOL_CURL_SWITCH_H


#include "tool/common/common.h"
#include "base/utility/uncopy.h"
#include "base/utility/singleton.h"

NAMESPACE_STUPID_TOOL_BEGIN

class STUPID_API CurlSwitch : private Stupid::Base::Uncopy
{
public:
    void work();

private:
    CurlSwitch();
    ~CurlSwitch();

    friend class Stupid::Base::Singleton<CurlSwitch>;
};

NAMESPACE_STUPID_TOOL_END


#endif // STUPID_TOOL_CURL_SWITCH_H
