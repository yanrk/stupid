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

#ifndef STUPID_BASE_ASYN_LOG_H
#define STUPID_BASE_ASYN_LOG_H


#include <list>
#include <vector>
#include "base/common/common.h"
#include "base/thread/thread.h"
#include "base/locker/locker.h"
#include "base/pool/object_pool.h"
#include "base/log/log_base.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API AsynLog : public LogBase
{
public:
    AsynLog(const std::string & path, const std::string & log_type, STUPID_LOG_LEVEL min_log_level, size_t max_file_size, bool output_to_console);
    virtual ~AsynLog();

public:
    static bool acquire_write_thread();
    static void release_write_thread();

    static void push_asyn_log_object(AsynLog * asyn_log);
    static void pop_asyn_log_object(AsynLog * asyn_log);

    static void write_file();

protected:
    virtual void save_record(STUPID_LOG_LEVEL log_level, const char * data, size_t size);

private:
    static bool try_write_file();

private:
    enum { ASYN_RECORD_SIZE = LOG_RECORD_SIZE * 8 };

    struct LOG_RECORD
    {
        char   data[ASYN_RECORD_SIZE];
        size_t size;
    };

private:
    static Thread                            s_write_file_thread;
    static std::list<AsynLog *>              s_asyn_log_obj_list;
    static ThreadLocker                      s_asyn_log_obj_locker;

private:
    ObjectPool<LOG_RECORD, ThreadLocker>     m_idle_pool;
    ObjectPool<LOG_RECORD, ThreadLocker>     m_data_pool;
    LOG_RECORD                             * m_current_record;
    ThreadLocker                             m_current_record_locker;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_ASYN_LOG_H
