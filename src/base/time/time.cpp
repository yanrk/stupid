/********************************************************
 * Description : time functions
 * Data        : 2013-05-24 12:15:59
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <sstream>
#include <iomanip>
#include "base/string/string.h"
#include "base/time/time.h"

NAMESPACE_STUPID_BASE_BEGIN

int64_t stupid_time()
{
    return(static_cast<int64_t>(time(nullptr)));
}

struct tm stupid_make_localtime(int64_t time_second)
{
    time_t time_value = static_cast<time_t>(time_second);
    struct tm tm_value;

#ifdef _MSC_VER
    localtime_s(&tm_value, &time_value);
#else
    localtime_r(&time_value, &tm_value);
#endif // _MSC_VER

    return(tm_value);
}

struct tm stupid_make_gmtime(int64_t time_second)
{
    time_t time_value = static_cast<time_t>(time_second);
    struct tm tm_value;

#ifdef _MSC_VER
    gmtime_s(&tm_value, &time_value);
#else
    gmtime_r(&time_value, &tm_value);
#endif // _MSC_VER

    return(tm_value);
}

int64_t stupid_make_time(struct tm tm_value)
{
    return(static_cast<int64_t>(mktime(&tm_value)));
}

struct tm stupid_localtime()
{
    return(stupid_make_localtime(stupid_time()));
}

struct tm stupid_gmtime()
{
    return(stupid_make_gmtime(stupid_time()));
}

struct timeval stupid_gettimeofday()
{
    struct timeval tv_now;

#ifdef _MSC_VER
    SYSTEMTIME sys_now;
    GetLocalTime(&sys_now);
    tv_now.tv_sec = static_cast<long>(time(nullptr));
    tv_now.tv_usec = sys_now.wMilliseconds * 1000L;
#else
    gettimeofday(&tv_now, nullptr);
#endif // _MSC_VER

    return(tv_now);
}

int stupid_get_timezone()
{
    long time_diff = 0;

#ifdef _MSC_VER
    _get_timezone(&time_diff);
#else
    time_diff = timezone;
#endif // _MSC_VER

    time_diff /= -3600;

    return(time_diff);
}

static int stupid_get_day_of_week(const struct tm & tm_now)
{
    return(tm_now.tm_wday);
}

int stupid_get_day_of_week()
{
    return(stupid_get_day_of_week(stupid_localtime()));
}

static std::string stupid_get_week(const struct tm & tm_now, bool week_abbreviation = true)
{
    const char * const week[][7] = 
    {
        { "Sun",    "Mon",    "Tue",     "Wed",       "Thu",      "Fri",    "Sat"      }, 
        { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" }  
    };
    return(std::string(week[week_abbreviation ? 0 : 1][stupid_get_day_of_week(tm_now)]));
}

std::string stupid_get_week(bool week_abbreviation)
{
    return(stupid_get_week(stupid_localtime(), week_abbreviation));
}

static std::string stupid_get_date(const struct tm & tm_now, const char * date_delimiter = "-")
{
    if (nullptr == date_delimiter)
    {
        date_delimiter = "-";
    }

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(4) << (tm_now.tm_year + 1900)
        << date_delimiter
        << std::setw(2) << (tm_now.tm_mon + 1)
        << date_delimiter
        << std::setw(2) << (tm_now.tm_mday);
    return(oss.str());
}

std::string stupid_get_date(const char * date_delimiter)
{
    return(stupid_get_date(stupid_localtime(), date_delimiter));
}

static std::string stupid_get_time(const struct tm & tm_now, const char * time_delimiter = ":")
{
    if (nullptr == time_delimiter)
    {
        time_delimiter = ":";
    }

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << (tm_now.tm_hour)
        << time_delimiter
        << std::setw(2) << (tm_now.tm_min)
        << time_delimiter
        << std::setw(2) << (tm_now.tm_sec);
    return(oss.str());
}

std::string stupid_get_time(const char * time_delimiter)
{
    return(stupid_get_time(stupid_localtime(), time_delimiter));
}

static std::string stupid_get_datetime(
                       const struct tm & tm_now, 
                       const char * date_delimiter = "-", 
                       const char * time_delimiter = ":"
                   )
{
    return(stupid_get_date(tm_now, date_delimiter) + " " + stupid_get_time(tm_now, time_delimiter));
}

std::string stupid_get_datetime(const char * date_delimiter, const char * time_delimiter)
{
    return(stupid_get_datetime(stupid_localtime(), date_delimiter, time_delimiter));
}

std::string stupid_get_comprehensive_datetime(
                const char * date_delimiter, 
                const char * time_delimiter, 
                bool week_abbreviation
            )
{
    struct timeval tv_now = stupid_gettimeofday();
    size_t time_ms = static_cast<size_t>(tv_now.tv_usec / 1000);
    int64_t time_now = static_cast<int64_t>(tv_now.tv_sec);
    struct tm tm_now = stupid_make_localtime(time_now);

    char buff[32] = { 0 };
    stupid_snprintf
    (
        buff, sizeof(buff), ".%03u GMT%+03d:00 ", 
        time_ms, stupid_get_timezone()
    );

    return(stupid_get_datetime(tm_now) + buff + stupid_get_week(tm_now));
}

void stupid_ms_sleep(size_t milliseconds)
{
    int64_t nanoseconds = STUPID_I64_VAL(1000000) * milliseconds;
    stupid_ns_sleep(nanoseconds);
}

void stupid_ns_sleep(int64_t nanoseconds)
{
#ifdef _MSC_VER
    Sleep(static_cast<DWORD>(nanoseconds / 1000000));
#else
    struct timespec sleep_time;
    sleep_time.tv_sec = static_cast<time_t>(nanoseconds / 1000000000);
    sleep_time.tv_nsec = static_cast<long>(nanoseconds % 1000000000);

    while (0 != nanosleep(&sleep_time, &sleep_time) && EINTR == errno)
    {
        /* continue */
    }
#endif // _MSC_VER
}

NAMESPACE_STUPID_BASE_END
