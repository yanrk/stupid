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

#ifdef _MSC_VER
    #include <sys/timeb.h>
#endif // _MSC_VER

#include <sstream>
#include <iomanip>
#include "base/string/string.h"
#include "base/time/time.h"

NAMESPACE_STUPID_BASE_BEGIN

uint64_t stupid_time()
{
    return(static_cast<uint64_t>(time(nullptr)));
}

struct tm stupid_make_localtime(uint64_t time_second)
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

struct tm stupid_make_gmtime(uint64_t time_second)
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

uint64_t stupid_make_time(struct tm tm_value)
{
    return(static_cast<uint64_t>(mktime(&tm_value)));
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
    timeb time_now;
    ftime(&time_now);
    tv_now.tv_sec = static_cast<long>(time_now.time);
    tv_now.tv_usec = static_cast<long>(time_now.millitm) * 1000L;
#else
    gettimeofday(&tv_now, nullptr);
#endif // _MSC_VER

    return(tv_now);
}

int goofer_get_timezone()
{
    long time_zone = 0;

#ifdef _MSC_VER
    _get_timezone(&time_zone);
#else
    time_zone = timezone;
#endif // _MSC_VER

    return (static_cast<int>(time_zone));
}

int stupid_get_day_of_week(const struct tm & tm_value)
{
    return(tm_value.tm_wday);
}

int stupid_get_day_of_week()
{
    return(stupid_get_day_of_week(stupid_localtime()));
}

std::string stupid_get_week(const struct tm & tm_value, bool week_abbreviation)
{
    const char * const week[][7] = 
    {
        { "Sun",    "Mon",    "Tue",     "Wed",       "Thu",      "Fri",    "Sat"      }, 
        { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" }  
    };
    return(std::string(week[week_abbreviation ? 0 : 1][stupid_get_day_of_week(tm_value)]));
}

std::string stupid_get_week(bool week_abbreviation)
{
    return(stupid_get_week(stupid_localtime(), week_abbreviation));
}

std::string stupid_get_date(const struct tm & tm_value, const char * date_delimiter)
{
    if (nullptr == date_delimiter)
    {
        date_delimiter = "-";
    }

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(4) << (tm_value.tm_year + 1900)
        << date_delimiter
        << std::setw(2) << (tm_value.tm_mon + 1)
        << date_delimiter
        << std::setw(2) << (tm_value.tm_mday);
    return(oss.str());
}

std::string stupid_get_date(const char * date_delimiter)
{
    return(stupid_get_date(stupid_localtime(), date_delimiter));
}

std::string stupid_get_time(const struct tm & tm_value, const char * time_delimiter)
{
    if (nullptr == time_delimiter)
    {
        time_delimiter = ":";
    }

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << (tm_value.tm_hour)
        << time_delimiter
        << std::setw(2) << (tm_value.tm_min)
        << time_delimiter
        << std::setw(2) << (tm_value.tm_sec);
    return(oss.str());
}

std::string stupid_get_time(const char * time_delimiter)
{
    return(stupid_get_time(stupid_localtime(), time_delimiter));
}

std::string stupid_datetime()
{
    time_t t_now = time(nullptr);
    struct tm tm_now = { 0x00 };

#ifdef _MSC_VER
    localtime_s(&tm_now, &t_now);
#else
    localtime_r(&t_now, &tm_now);
#endif // _MSC_VER

    char str_time[32] = { 0x00 };
    strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S", &tm_now);

    return(str_time);
}

std::string stupid_get_datetime(const struct tm & tm_value, const char * date_delimiter, const char * time_delimiter, const char * date_time_delimiter)
{
    if (nullptr == date_time_delimiter)
    {
        date_time_delimiter = " ";
    }
    return(stupid_get_date(tm_value, date_delimiter) + date_time_delimiter + stupid_get_time(tm_value, time_delimiter));
}

std::string stupid_get_datetime(const char * date_delimiter, const char * time_delimiter, const char * date_time_delimiter)
{
    return(stupid_get_datetime(stupid_localtime(), date_delimiter, time_delimiter, date_time_delimiter));
}

std::string stupid_get_comprehensive_datetime(
                const char * date_delimiter, 
                const char * time_delimiter, 
                const char * date_time_delimiter, 
                bool week_abbreviation
            )
{
    struct timeval tv_now = stupid_gettimeofday();
    size_t time_ms = static_cast<size_t>(tv_now.tv_usec / 1000);
    uint64_t time_now = static_cast<uint64_t>(tv_now.tv_sec);
    struct tm tm_now = stupid_make_localtime(time_now);

    char buff[32] = { 0 };
    stupid_snprintf
    (
        buff, sizeof(buff), ".%03u GMT%+03d:00 ", 
        time_ms, stupid_get_timezone() / -3600
    );

    return(stupid_get_datetime(tm_now, date_delimiter, time_delimiter, date_time_delimiter) + buff + stupid_get_week(tm_now));
}

void stupid_ms_sleep(size_t milliseconds)
{
    uint64_t nanoseconds = STUPID_U64_VAL(1000000) * milliseconds;
    stupid_ns_sleep(nanoseconds);
}

void stupid_ns_sleep(uint64_t nanoseconds)
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
