/********************************************************
 * Description : log switch (assistant of log module)
 * Data        : 2013-10-19 10:56:24
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include "base/log/log.h"
#include "base/utility/log_switch.h"

NAMESPACE_STUPID_BASE_BEGIN

LogSwitch::LogSwitch()
    : m_init(false)
{

}

LogSwitch::~LogSwitch()
{
    exit();
}

bool LogSwitch::init(const char * config_file)
{
    if (nullptr == config_file || '\0' == config_file[0])
    {
        config_file = "../../config/log.ini";
    }

    LOG_CONFIG log_config;
    const std::string config_name(config_file);

    if (!log_load_config(config_name, log_config))
    {
        log_config.log_file_path = "./log/";
        for (size_t type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
        {
            log_config.log_file[type].write_mode = LAZY_WRITE_MODE;
            log_config.log_file[type].log_min_level = TRK_LEVEL;
            log_config.log_file[type].log_file_size = 10;
            log_config.log_file[type].buffer_count = 50;
            log_config.log_file[type].output_to_console = false;
        }
    }

    return(init(log_config));
}

bool LogSwitch::init(const LOG_CONFIG & log_config)
{
    if (m_init)
    {
        return(true);
    }

    m_init = log_init(log_config);

    return(m_init);
}

void LogSwitch::exit()
{
    if (!m_init)
    {
        return;
    }

    log_exit();

    m_init = false;
}

void LogSwitch::set_min_level(LOG_TYPE log_type, LOG_LEVEL log_level)
{
    log_set_level(log_type, log_level);
}

void LogSwitch::set_console_output_switch(LOG_TYPE log_type, bool output_to_console)
{
    log_set_console(log_type, output_to_console);
}

NAMESPACE_STUPID_BASE_END
