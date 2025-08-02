/********************************************************
 * Description : message record
 * Data        : 2017-03-24 11:30:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "base/time/time.h"
#include "base/string/string.h"
#include "base/utility/guard.h"
#include "base/utility/utility.h"
#include "base/filesystem/directory.h"
#include "base/record/record.h"

NAMESPACE_STUPID_BASE_BEGIN

Record::Record()
    : m_is_running(false)
    , m_max_filesize(1024 * 1024 * 10)
    , m_success_dirname()
    , m_success_filename()
    , m_success_file()
    , m_success_filesize(0)
    , m_success_file_locker()
    , m_failure_dirname()
    , m_failure_filename()
    , m_failure_file()
    , m_failure_filesize(0)
    , m_failure_file_locker()
{

}

Record::~Record()
{

}

bool Record::init(const std::string & record_root_directory)
{
    if (m_is_running)
    {
        return true;
    }

    m_success_dirname = record_root_directory + "/success_record/";
    stupid_create_directory_recursive(m_success_dirname);
    m_success_filename = m_success_dirname + "success.log";
    if (!file_get_size(m_success_filename.c_str(), m_success_filesize))
    {
        m_success_filesize = 0;
    }

    m_failure_dirname = record_root_directory + "/failure_record/";
    stupid_create_directory_recursive(m_failure_dirname);
    m_failure_filename = m_failure_dirname + "failure.log";
    if (!file_get_size(m_failure_filename.c_str(), m_failure_filesize))
    {
        m_failure_filesize = 0;
    }

    if (!m_success_file.open(m_success_filename.c_str(), true, false))
    {
        return false;
    }
    if (!m_failure_file.open(m_failure_filename.c_str(), true, false))
    {
        return false;
    }

    m_is_running = true;

    return true;
}

void Record::exit()
{
    if (m_is_running)
    {
        m_is_running = false;
        m_success_file.close();
        m_failure_file.close();
    }
}

bool Record::running() const
{
    return m_is_running;
}

void Record::write(const char * data, size_t size, bool success_record)
{
    if (!m_is_running)
    {
        return;
    }

    if (success_record)
    {
        write_success_record(data, size);
    }
    else
    {
        write_failure_record(data, size);
    }
}

void Record::write_success_record(const char * data, size_t size)
{
    Guard<ThreadLocker> success_guard(m_success_file_locker);

    if (!m_success_file.write(data, size))
    {
        return;
    }

    m_success_filesize += size;
    if (m_success_filesize < m_max_filesize)
    {
        return;
    }

    m_success_file.close();

    const std::string date_time = stupid_get_datetime("", "", "");

    const std::string date_dirname(m_success_dirname + date_time.substr(0, 8) + "/");
    stupid_mkdir_safe(date_dirname.c_str());

    const std::string datetime_filename(date_dirname + "success_" + date_time + ".log");
    stupid_rename_safe(m_success_filename.c_str(), datetime_filename.c_str());

    m_success_file.open(m_success_filename.c_str(), true, false);
    m_success_filesize = 0;
}

void Record::write_failure_record(const char * data, size_t size)
{
    Guard<ThreadLocker> failure_guard(m_failure_file_locker);

    if (!m_failure_file.write(data, size))
    {
        return;
    }

    m_failure_filesize += size;
    if (m_failure_filesize < m_max_filesize)
    {
        return;
    }

    m_failure_file.close();

    const std::string date_time = stupid_get_datetime("", "", "");

    const std::string date_dirname(m_failure_dirname + date_time.substr(0, 8) + "/");
    stupid_mkdir_safe(date_dirname.c_str());

    const std::string datetime_filename(date_dirname + "failure_" + date_time + ".log");
    stupid_rename_safe(m_failure_filename.c_str(), datetime_filename.c_str());

    m_failure_file.open(m_failure_filename.c_str(), true, false);
    m_failure_filesize = 0;
}

NAMESPACE_STUPID_BASE_END

static void do_stupid_record(Stupid::Base::Record & record, bool success_record, const char * format, va_list args)
{
    if (nullptr == format || nullptr == args)
    {
        return;
    }

    if (!record.running())
    {
        return;
    }

    char message[2048] = { 0 };
    size_t message_size = 0;

    message_size += Stupid::Base::stupid_snprintf
    (
        message + message_size, sizeof(message) - message_size, 
        "%s", Stupid::Base::stupid_get_comprehensive_datetime("-", ":", " ", true).c_str()
    );

    message_size += Stupid::Base::stupid_snprintf
    (
        message + message_size, sizeof(message) - message_size, 
        " | %s | ", (success_record ? "TRACE" : "ERROR")
    );

    message_size += Stupid::Base::stupid_vsnprintf
    (
        message + message_size, sizeof(message) - message_size, format, args
    );

    message_size += 1; /* for '\n' or '\0' */
    if (message_size > sizeof(message))
    {
        message_size = sizeof(message);
    }

    message[message_size - 1] = '\n';

    record.write(message, message_size, success_record);
}

void stupid_record(Stupid::Base::Record & record, bool success_record, const char * format, ...)
{
    va_list args;

    va_start(args, format);

    do_stupid_record(record, success_record, format, args);

    va_end(args);
}
