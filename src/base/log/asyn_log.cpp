/********************************************************
 * Description : log class of asyn mode
 * Data        : 2013-05-23 20:12:41
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
#include "base/time/time.h"
#include "base/log/asyn_log.h"

NAMESPACE_STUPID_BASE_BEGIN

static thread_return_t STUPID_STDCALL write_file_run(thread_argument_t argument = nullptr)
{
    argument = nullptr; /* prevent warning */
    AsynLog::write_file();
    return(THREAD_DEFAULT_RET);
}

Thread AsynLog::s_write_file_thread;
std::list<AsynLog *> AsynLog::s_asyn_log_obj_list;
ThreadLocker AsynLog::s_asyn_log_obj_locker;

AsynLog::AsynLog(const std::string & path, const std::string & log_type, STUPID_LOG_LEVEL min_log_level, size_t max_file_size, bool output_to_console)
    : LogBase(path, log_type, min_log_level, max_file_size, output_to_console)
    , m_idle_pool("idle pool of asyn log", "idle pool locker of asyn log")
    , m_data_pool("data pool of asyn log", "data pool locker of asyn log")
    , m_current_record(nullptr)
    , m_current_record_locker("current record locker of asyn log")
{
    m_current_record = m_idle_pool.acquire();
    if (nullptr != m_current_record)
    {
        m_current_record->size = 0;
    }
    push_asyn_log_object(this);
}

AsynLog::~AsynLog()
{
    pop_asyn_log_object(this);
    Guard<ThreadLocker> record_guard(m_current_record_locker);
    m_idle_pool.release(m_current_record);
}

void AsynLog::save_record(STUPID_LOG_LEVEL log_level, const char * data, size_t size)
{
    Guard<ThreadLocker> record_guard(m_current_record_locker);

    log_level = LOG_LEVEL_MAX; /* prevent warning */

    if (nullptr == m_current_record)
    {
        m_current_record = m_idle_pool.acquire();
        if (nullptr == m_current_record)
        {
            assert(false);
            return;
        }
        m_current_record->size = 0;
    }

    if (ASYN_RECORD_SIZE < size)
    {
        size = ASYN_RECORD_SIZE;
    }

    if (ASYN_RECORD_SIZE < m_current_record->size + size)
    {
        m_data_pool.release(m_current_record);
        m_current_record = m_idle_pool.acquire();
        if (nullptr == m_current_record)
        {
            assert(false);
            return;
        }
        m_current_record->size = 0;
    }

    memcpy(m_current_record->data + m_current_record->size, data, size);
    m_current_record->size += size;
}

bool AsynLog::acquire_write_thread()
{
    s_write_file_thread.set_thread_args(write_file_run, &s_asyn_log_obj_list, "write file thread of asyn log");
    return(s_write_file_thread.acquire());
}

void AsynLog::release_write_thread()
{
    s_write_file_thread.release();
}

void AsynLog::push_asyn_log_object(AsynLog * asyn_log)
{
    Guard<ThreadLocker> obj_list_guard(s_asyn_log_obj_locker);

    if (nullptr != asyn_log)
    {
        s_asyn_log_obj_list.push_back(asyn_log);
    }
}

void AsynLog::pop_asyn_log_object(AsynLog * asyn_log)
{
    Guard<ThreadLocker> obj_list_guard(s_asyn_log_obj_locker);

    if (nullptr != asyn_log)
    {
        s_asyn_log_obj_list.remove(asyn_log);
    }
}

bool AsynLog::try_write_file()
{
    Guard<ThreadLocker> obj_list_guard(s_asyn_log_obj_locker);

    if (s_asyn_log_obj_list.empty())
    {
        return(false);
    }

    bool has_data = false;

    std::list<AsynLog *>::iterator obj_iter = s_asyn_log_obj_list.begin();
    for (; s_asyn_log_obj_list.end() != obj_iter; ++obj_iter)
    {
        AsynLog * asyn_log = *obj_iter;
        if (nullptr == asyn_log)
        {
            assert(nullptr != asyn_log);
            continue;
        }

        std::list<LOG_RECORD *> record_list;
        asyn_log->m_data_pool >> record_list;

        std::list<LOG_RECORD *>::iterator record_iter = record_list.begin();
        while (record_list.end() != record_iter)
        {
            LOG_RECORD * log_record = *record_iter;
            if (nullptr != log_record)
            {
                if (log_record->size > 0)
                {
                    has_data = true;
                }
                asyn_log->write(log_record->data, log_record->size);
            }
            else
            {
                assert(false);
            }
            ++record_iter;
        }

        asyn_log->m_idle_pool << record_list;

        Guard<ThreadLocker> record_guard(asyn_log->m_current_record_locker);
        if (0 == asyn_log->m_data_pool.size())
        {
            LOG_RECORD * current_record = asyn_log->m_current_record;
            if (nullptr != current_record)
            {
                if (current_record->size > 0)
                {
                    has_data = true;
                }
                asyn_log->write(current_record->data, current_record->size);
                current_record->size = 0;
            }
        }
    }

    return(has_data);
}

void AsynLog::write_file()
{
    while (s_write_file_thread.running())
    {
        if (!try_write_file())
        {
            stupid_ms_sleep(10);
        }
    }
    try_write_file();
}

NAMESPACE_STUPID_BASE_END
