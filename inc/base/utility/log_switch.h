/********************************************************
 * Description : log switch (assistant of log module)
 * Data        : 2013-10-19 10:56:24
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_LOG_SWITCH_H
#define STUPID_BASE_LOG_SWITCH_H


#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/utility/singleton.h"
#include "base/log/log_types.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API LogSwitch : private Uncopy
{
public:
    bool init(const char * config_file = nullptr);
    bool init(const STUPID_LOG_CONFIG & log_config);
    void exit();

public:
    void enable(STUPID_LOG_TYPE log_type);
    void disable(STUPID_LOG_TYPE log_type);
    void enable_all();
    void disable_all();
    void set_min_level(STUPID_LOG_TYPE log_type, STUPID_LOG_LEVEL log_level);
    void set_console_output_switch(STUPID_LOG_TYPE log_type, bool output_to_console);

private:
    LogSwitch();
    ~LogSwitch();

private:
    friend class Singleton<LogSwitch>;

private:
    bool                   m_init;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_LOG_SWITCH_H
