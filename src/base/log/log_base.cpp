/********************************************************
 * Description : base class of log classes
 * Data        : 2013-05-23 09:24:56
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <pthread.h>
#endif // _MSC_VER

#include <fstream>
#include "base/utility/guard.h"
#include "base/utility/utility.h"
#include "base/filesystem/directory.h"
#include "base/time/time.h"
#include "base/string/string.h"
#include "base/log/log_base.h"

NAMESPACE_STUPID_BASE_BEGIN

LogBase::LogBase(const std::string & path, const std::string & log_type, STUPID_LOG_LEVEL min_log_level, size_t max_file_size, bool output_to_console)
    : m_dirname(path + g_directory_separator + log_type + g_directory_separator)
    , m_log_type(log_type)
    , m_filename(m_dirname + m_log_type + ".log")
    , m_file()
    , m_min_log_level(min_log_level)
    , m_cur_file_size(0)
    , m_max_file_size(max_file_size * 1024 * 1024)
    , m_file_locker("file locker of base log")
    , m_output_to_console(output_to_console)
{
    stupid_create_directory_recursive(m_dirname);
    m_file.open(m_filename.c_str(), true, false);
    if (!file_get_size(m_filename.c_str(), m_cur_file_size))
    {
        m_cur_file_size = 0;
    }
}

LogBase::~LogBase()
{
    m_file.close();
}

void LogBase::push_record(STUPID_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, va_list args)
{
    if (nullptr == file || nullptr == func || nullptr == format || nullptr == args)
    {
        return;
    }

    if (level > m_min_log_level)
    {
        return;
    }

    char record[LOG_RECORD_SIZE];
    size_t record_size = 0;

    record_size += stupid_snprintf
    (
        record + record_size, LOG_RECORD_SIZE - record_size, 
        "%s", stupid_get_comprehensive_datetime("-", ":", " ", true).c_str()
    );

    record_size += stupid_snprintf
    (
        record + record_size, LOG_RECORD_SIZE - record_size, 
        " | %s | T%010u | %s:%s:%05d | ", 
        STUPID_LOG_LEVEL_INFO[level], get_tid(), file, func, line
    );

    record_size += stupid_vsnprintf
    (
        record + record_size, LOG_RECORD_SIZE - record_size, format, args
    );

    record_size += 1; /* for '\n' or '\0' */
    if (LOG_RECORD_SIZE < record_size)
    {
        record_size = LOG_RECORD_SIZE;
    }

    record[record_size - 1] = '\0';

#ifdef _MSC_VER
//  OutputDebugStringA(record);
#endif // _MSC_VER

    if (m_output_to_console)
    {
        printf("%s\n", record);
    }

    record[record_size - 1] = '\n';
    save_record(level, record, record_size);
}

void LogBase::set_min_level(STUPID_LOG_LEVEL level)
{
    m_min_log_level = level;
}

void LogBase::set_console_output_switch(bool output_to_console)
{
    m_output_to_console = output_to_console;
}

void LogBase::write(const char * data, size_t size)
{
    Guard<ThreadLocker> file_guard(m_file_locker);

    if (!m_file.write(data, size))
    {
        return;
    }

    m_cur_file_size += size;
    if (m_cur_file_size < m_max_file_size)
    {
        return;
    }

    m_file.close();

    const std::string date_time = stupid_get_datetime("", "", "");

    const std::string date_dirname(m_dirname + date_time.substr(0, 8) + g_directory_separator);
    stupid_mkdir_safe(date_dirname.c_str());

    const std::string datetime_filename(date_dirname + m_log_type + "_" + date_time + ".log");
    stupid_rename_safe(m_filename.c_str(), datetime_filename.c_str());

    m_file.open(m_filename.c_str(), true, true);
    m_cur_file_size = 0;
}

NAMESPACE_STUPID_BASE_END
