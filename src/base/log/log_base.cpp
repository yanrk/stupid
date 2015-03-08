/********************************************************
 * Description : base class of log classes
 * Data        : 2013-05-23 09:24:56
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <pthread.h>
#endif // _MSC_VER

#include <fstream>
#include "base/utility/guard.h"
#include "base/utility/utility.h"
#include "base/filesystem/sys_io.h"
#include "base/time/time.h"
#include "base/string/string.h"
#include "base/log/log_base.h"

NAMESPACE_STUPID_BASE_BEGIN

LogBase::LogBase(const std::string & path, const std::string & log_type, LOG_LEVEL min_log_level, size_t max_file_size, bool output_to_console)
    : m_path(path + "/" + log_type + "/")
    , m_log_type(log_type)
    , m_fullname(m_path + m_log_type + ".log")
    , m_file(-1)
    , m_min_log_level(min_log_level)
    , m_cur_file_size(0)
    , m_max_file_size(max_file_size * 1024 * 1024)
    , m_file_locker("file locker of base log")
    , m_output_to_console(output_to_console)
{
    create_path();
    open();
}


LogBase::~LogBase()
{
    close();
}

void LogBase::create_path()
{
    std::string::size_type pos = 0;
    while (std::string::npos != (pos = m_path.find_first_of("/\\", pos)))
    {
        pos += 1;
        std::string dir(m_path, 0, pos);
        stupid_mkdir(dir.c_str());
    }
    stupid_mkdir(m_path.c_str());
}

void LogBase::open()
{
    close();
    stupid_umask();
    m_file = stupid_open(m_fullname.c_str(), O_WRONLY | O_CREAT | O_APPEND);
    update_file_size();
}

void LogBase::close()
{
    if (-1 != m_file)
    {
        stupid_close(m_file);
        m_file = -1;
    }
}

void LogBase::rename()
{
    std::ifstream ifs(m_fullname);
    if (!ifs.is_open())
    {
        return;
    }

    const size_t LINE_MAX_SIZE = 2048;

    ifs.seekg(0, std::ios::end);
    size_t len = static_cast<size_t>(ifs.tellg());

    size_t offset = 0;
    if (LINE_MAX_SIZE < len)
    {
        offset = len - LINE_MAX_SIZE;
    }
    ifs.seekg(offset, std::ios::beg);

    std::string datetime;
    char line[LINE_MAX_SIZE];
    while(!ifs.eof())
    {
        ifs.getline(line, LINE_MAX_SIZE);
        size_t size = static_cast<size_t>(ifs.gcount());
        line[size] = '\0';
        parse_datetime(line, size + 1, datetime);
    }

    ifs.close();

    if (datetime.empty())
    {
        datetime = stupid_get_datetime("", "", "");
    }

    std::string new_dir(m_path + datetime.substr(0, 8) + "/");
    stupid_mkdir(new_dir.c_str());

    std::string new_name(new_dir + m_log_type + "_" + datetime + ".log");
    if (0 != stupid_rename(m_fullname.c_str(), new_name.c_str()))
    {
        std::ofstream(new_name) << ifs.rdbuf();
        std::ofstream(m_fullname, std::ios::trunc);
    }
}

void LogBase::update_file_size()
{
    std::ifstream ifs(m_fullname);
    if (!ifs.is_open())
    {
        return;
    }
    ifs.seekg(0, std::ios::end);
    m_cur_file_size = static_cast<size_t>(ifs.tellg());
    ifs.close();
}

void LogBase::parse_datetime(const char * record, size_t size, std::string & datetime)
{
    if (nullptr == record || 20 > size)
    {
        return;
    }

    int year   = 0;
    int month  = 0;
    int day    = 0;
    int hour   = 0;
    int minute = 0;
    int second = 0;

    int ret = sscanf(record, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);

    if (6 != ret || 1900 > year || 0 >= month || 12 < month || 
        0 >= day || 31 < day || 0 > hour || 24 <= hour || 
        0 > minute || 60 <= minute || 0 > second || 60 <= second)
    {
        return;
    }

    char date_time[19] = { 0 };
    size_t temp_size = 0;
    for (size_t index = 0; index < 19; ++index)
    {
        if ('0' <= record[index] && '9' >= record[index])
        {
            date_time[temp_size] = record[index];
            ++temp_size;
        }
    }
    std::string(date_time, date_time + temp_size).swap(datetime);
}

void LogBase::push_record(LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, va_list args)
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
        LOG_LEVEL_INFO[level], get_tid(), file, func, line
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

#ifdef _MSC_VER
    record[record_size - 1] = '\0';
    OutputDebugStringA(record);
#endif // _MSC_VER

    if (m_output_to_console)
    {
        record[record_size - 1] = '\0';
        printf("%s\n", record);
    }

    record[record_size - 1] = '\n';
    save_record(level, record, record_size);
}

void LogBase::push_record(LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...)
{
    va_list args = nullptr;

    va_start(args, format);

    push_record(level, file, func, line, format, args);

    va_end(args);
}

void LogBase::set_min_level(LOG_LEVEL level)
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

    if (-1 == m_file || -1 == stupid_write(m_file, data, size))
    {
        return;
    }

    m_cur_file_size += size;
    if (m_cur_file_size < m_max_file_size)
    {
        return;
    }

    close();
    rename();
    open();
}

NAMESPACE_STUPID_BASE_END
