/********************************************************
 * Description : log class of lazy mode
 * Data        : 2013-05-23 13:56:44
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cassert>
#include <cstring>
#include "base/utility/guard.h"
#include "base/log/lazy_log.h"

NAMESPACE_STUPID_BASE_BEGIN

LazyLog::LazyLog(const std::string & path, const std::string & log_type, STUPID_LOG_LEVEL min_log_level, size_t max_file_size, size_t max_record_count, bool output_to_console)
    : LogBase(path, log_type, min_log_level, max_file_size, output_to_console)
    , m_record(nullptr)
    , m_reserve(max_record_count)
    , m_begin(0)
    , m_count(0)
    , m_record_locker("record locker of lazy log")
{
    STUPID_NEW_ARR(m_record, LOG_RECORD, max_record_count);
    assert(nullptr != m_record);
}

LazyLog::~LazyLog()
{
    STUPID_DEL_ARR(m_record);
}

void LazyLog::save_record(STUPID_LOG_LEVEL log_level, const char * data, size_t size)
{
    Guard<ThreadLocker> record_guard(m_record_locker);

    if (nullptr == m_record)
    {
        return;
    }

    if (size > LOG_RECORD_SIZE)
    {
        size = LOG_RECORD_SIZE;
    }

    size_t push_index = (m_begin + m_count) % m_reserve;
    memcpy(m_record[push_index].data, data, size);
    m_record[push_index].size = size;
    m_record[push_index].data[size - 1] = '\n';

    if (m_count < m_reserve)
    {
        ++m_count;
    }

    if (ERR_LEVEL >= log_level)
    {
        flush();
    }
}

void LazyLog::flush()
{
    for (size_t index = 0; index < m_count; ++index)
    {
        size_t record_index = (m_begin + index) % m_reserve;
        write(m_record[record_index].data, m_record[record_index].size);
    }
    m_begin = 0;
    m_count = 0;
}

NAMESPACE_STUPID_BASE_END
