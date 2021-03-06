/********************************************************
 * Description : time functions
 * Data        : 2013-05-24 12:15:59
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_TIME_H
#define STUPID_BASE_TIME_H


#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <sys/time.h>
    #include <unistd.h>
    #include <errno.h>
#endif // _MSC_VER

#include <ctime>
#include <cstdint>
#include <string>
#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

STUPID_CXX_API(uint64_t) stupid_time();
STUPID_CXX_API(struct tm) stupid_localtime();
STUPID_CXX_API(struct tm) stupid_gmtime();
STUPID_CXX_API(struct tm) stupid_make_localtime(uint64_t time_second);
STUPID_CXX_API(struct tm) stupid_make_gmtime(uint64_t time_second);
STUPID_CXX_API(uint64_t) stupid_make_time(struct tm tm_value);
STUPID_CXX_API(struct timeval) stupid_gettimeofday();
STUPID_CXX_API(int) stupid_get_timezone();
STUPID_CXX_API(int) stupid_get_day_of_week();
STUPID_CXX_API(int) stupid_get_day_of_week(const struct tm & tm_value);
STUPID_CXX_API(std::string) stupid_get_week(bool week_abbreviation = true);
STUPID_CXX_API(std::string) stupid_get_week(const struct tm & tm_value, bool week_abbreviation = true);
STUPID_CXX_API(std::string) stupid_get_date(const char * date_delimiter = "-");
STUPID_CXX_API(std::string) stupid_get_date(const struct tm & tm_value, const char * date_delimiter = "-");
STUPID_CXX_API(std::string) stupid_get_time(const char * time_delimiter = ":");
STUPID_CXX_API(std::string) stupid_get_time(const struct tm & tm_value, const char * time_delimiter = ":");
STUPID_CXX_API(std::string) stupid_datetime();
STUPID_CXX_API(std::string) stupid_get_datetime(const char * date_delimiter = "-", const char * time_delimiter = ":", const char * date_time_delimiter = " ");
STUPID_CXX_API(std::string) stupid_get_datetime(const struct tm & tm_value, const char * date_delimiter = "-", const char * time_delimiter = ":", const char * date_time_delimiter = " ");
STUPID_CXX_API(std::string) stupid_get_comprehensive_datetime(const char * date_delimiter = "-", const char * time_delimiter = ":", const char * date_time_delimiter = " ", bool week_abbreviation = true);
STUPID_CXX_API(void) stupid_ms_sleep(size_t milliseconds);
STUPID_CXX_API(void) stupid_ns_sleep(uint64_t nanoseconds);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_TIME_H
