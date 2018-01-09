/********************************************************
 * Description : interface of log module
 * Data        : 2013-05-22 22:34:24
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_LOG_H
#define STUPID_BASE_LOG_H


#include <cstdarg>
#include <string>
#include "base/common/common.h"
#include "base/log/log_types.h"

STUPID_CXX_API(bool) stupid_log_load_config(const std::string & file, STUPID_LOG_CONFIG & log_config);
STUPID_CXX_API(bool) stupid_log_init(const STUPID_LOG_CONFIG & log_config);
STUPID_CXX_API(void) stupid_log_exit();
STUPID_CXX_API(void) stupid_log_enable(STUPID_LOG_TYPE log_type);
STUPID_CXX_API(void) stupid_log_disable(STUPID_LOG_TYPE log_type);
STUPID_CXX_API(void) stupid_log_enable_all();
STUPID_CXX_API(void) stupid_log_disable_all();
STUPID_CXX_API(void) stupid_log_set_level(STUPID_LOG_TYPE log_type, STUPID_LOG_LEVEL log_level);
STUPID_CXX_API(void) stupid_log_set_console(STUPID_LOG_TYPE log_type, bool output_to_console);
STUPID_CXX_API(void) stupid_run_log(STUPID_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...);
STUPID_CXX_API(void) stupid_debug_log(STUPID_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...);

#define RUN_LOG(level, fmt, ...)                                            \
stupid_run_log((level), __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define RUN_LOG_CRI(fmt, ...) RUN_LOG(CRI_LEVEL, (fmt), ##__VA_ARGS__)
#define RUN_LOG_ERR(fmt, ...) RUN_LOG(ERR_LEVEL, (fmt), ##__VA_ARGS__)
#define RUN_LOG_WAR(fmt, ...) RUN_LOG(WAR_LEVEL, (fmt), ##__VA_ARGS__)
#define RUN_LOG_DBG(fmt, ...) RUN_LOG(DBG_LEVEL, (fmt), ##__VA_ARGS__)
#define RUN_LOG_TRK(fmt, ...) RUN_LOG(TRK_LEVEL, (fmt), ##__VA_ARGS__)

#define DBG_LOG(fmt, ...)                                                       \
stupid_debug_log(DBG_LEVEL, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)


#endif // STUPID_BASE_LOG_H
