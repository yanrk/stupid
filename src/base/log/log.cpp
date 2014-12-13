/********************************************************
 * Description : interface of log module
 * Data        : 2013-05-22 22:34:24
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <cassert>
#include <string>
#include "base/config/ini.h"
#include "base/log/log_types.h"
#include "base/log/log_base.h"
#include "base/log/lazy_log.h"
#include "base/log/sync_log.h"
#include "base/log/asyn_log.h"
#include "base/log/log.h"

USING_NAMESPACE_STUPID_BASE

static bool transform_log_write_mode(const std::string & key_value, LOG_WRITE_MODE & write_mode)
{
    if ("SYNC_WRITE_MODE" == key_value)
    {
        write_mode = SYNC_WRITE_MODE;
    }
    else if ("ASYN_WRITE_MODE" == key_value)
    {
        write_mode = ASYN_WRITE_MODE;
    }
    else if ("LAZY_WRITE_MODE" == key_value)
    {
        write_mode = LAZY_WRITE_MODE;
    }
    else
    {
        return(false);
    }

    return(true);
}

static bool transform_log_level(const std::string & key_value, LOG_LEVEL & log_level)
{
    if ("CRI_LEVEL" == key_value)
    {
        log_level = CRI_LEVEL;
    }
    else if ("ERR_LEVEL" == key_value)
    {
        log_level = ERR_LEVEL;
    }
    else if ("WAR_LEVEL" == key_value)
    {
        log_level = WAR_LEVEL;
    }
    else if ("DBG_LEVEL" == key_value)
    {
        log_level = DBG_LEVEL;
    }
    else if ("TRK_LEVEL" == key_value)
    {
        log_level = TRK_LEVEL;
    }
    else
    {
        return(false);
    }

    return(true);
}

bool log_load_config(const std::string & file, LOG_CONFIG & log_config)
{
    Ini log_ini;
    if (!log_ini.load(file))
    {
        return(false);
    }

    if (!log_ini.get_value("", "log_path", log_config.log_file_path))
    {
        return(false);
    }

    for (int type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
    {
        std::string app_name(LOG_TYPE_INFO[type]);
        std::string key_value;

        if (!log_ini.get_value(app_name, "write_mode", key_value))
        {
            return(false);
        }

        if (!transform_log_write_mode(key_value, log_config.log_file[type].write_mode))
        {
            return(false);
        }

        if (!log_ini.get_value(app_name, "min_level", key_value))
        {
            return(false);
        }

        if (!transform_log_level(key_value, log_config.log_file[type].log_min_level))
        {
            return(false);
        }

        if (!log_ini.get_value(app_name, "file_size", log_config.log_file[type].log_file_size))
        {
            return(false);
        }

        if (!log_ini.get_value(app_name, "buffer_count", log_config.log_file[type].buffer_count))
        {
            return(false);
        }

        if (!log_ini.get_value(app_name, "output_to_console", log_config.log_file[type].output_to_console))
        {
            return(false);
        }
    }

    return(true);
}

static LogBase * s_log[LOG_TYPE_MAX] = { nullptr };
static bool      s_need_aysn_write_thread = false;

bool log_init(const LOG_CONFIG & log_config)
{
    for (size_t type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
    {
        switch (log_config.log_file[type].write_mode)
        {
            case SYNC_WRITE_MODE:
            {
                STUPID_NEW
                (
                    s_log[type], 
                    SyncLog
                    (
                        log_config.log_file_path, 
                        LOG_TYPE_INFO[type], 
                        log_config.log_file[type].log_min_level, 
                        log_config.log_file[type].log_file_size, 
                        log_config.log_file[type].output_to_console
                    )
                );
                break;
            }
            case ASYN_WRITE_MODE:
            {
                STUPID_NEW
                (
                    s_log[type], 
                    AsynLog
                    (
                        log_config.log_file_path, 
                        LOG_TYPE_INFO[type], 
                        log_config.log_file[type].log_min_level, 
                        log_config.log_file[type].log_file_size, 
                        log_config.log_file[type].output_to_console
                    )
                );
                s_need_aysn_write_thread = true;
                break;
            }
            case LAZY_WRITE_MODE:
            {
                STUPID_NEW
                (
                    s_log[type], 
                    LazyLog
                    (
                        log_config.log_file_path, 
                        LOG_TYPE_INFO[type], 
                        log_config.log_file[type].log_min_level, 
                        log_config.log_file[type].log_file_size, 
                        log_config.log_file[type].buffer_count, 
                        log_config.log_file[type].output_to_console
                    )
                );
                break;
            }
            default:
            {
                assert(false);
                return(false);
            }
        }

        if (nullptr == s_log[type])
        {
            assert(nullptr != s_log[type]);
            return(false);
        }
    }

    if (s_need_aysn_write_thread)
    {
        return(AsynLog::acquire_write_thread());
    }

    return(true);
}

void log_exit()
{
    if (s_need_aysn_write_thread)
    {
        AsynLog::release_write_thread();
        s_need_aysn_write_thread = false;
    }

    for (size_t type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
    {
        STUPID_DEL(s_log[type]);
    }
}

void log_set_level(LOG_TYPE log_type, LOG_LEVEL log_level)
{
    if (log_type < LOG_TYPE_MIN || log_type >= LOG_TYPE_MAX)
    {
        return;
    }

    if (nullptr != s_log[log_type])
    {
        s_log[log_type]->set_min_level(log_level);
    }
}

void log_set_console(LOG_TYPE log_type, bool output_to_console)
{
    if (log_type < LOG_TYPE_MIN || log_type >= LOG_TYPE_MAX)
    {
        return;
    }

    if (nullptr != s_log[log_type])
    {
        s_log[log_type]->set_console_output_switch(output_to_console);
    }
}

void run_log(LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...)
{
    va_list args = nullptr;

    va_start(args, format);

    if (nullptr != s_log[LOG_TYPE_RUN])
    {
        s_log[LOG_TYPE_RUN]->push_record(level, file, func, line, format, args);
    }
    if (nullptr != s_log[LOG_TYPE_DBG])
    {
        s_log[LOG_TYPE_DBG]->push_record(level, file, func, line, format, args);
    }

    va_end(args);
}

void debug_log(LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...)
{
    if (nullptr == s_log[LOG_TYPE_DBG])
    {
        return;
    }

    va_list args = nullptr;

    va_start(args, format);

    s_log[LOG_TYPE_DBG]->push_record(level, file, func, line, format, args);

    va_end(args);
}
