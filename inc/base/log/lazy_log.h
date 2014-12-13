/********************************************************
 * Description : log class of lazy mode
 * Data        : 2013-05-23 13:56:44
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_LAZY_LOG_H
#define STUPID_BASE_LAZY_LOG_H


#include "base/common/common.h"
#include "base/locker/locker.h"
#include "base/log/log_base.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API LazyLog : public LogBase
{
public:
    LazyLog(const std::string & path, const std::string & log_type, LOG_LEVEL min_log_level, size_t max_file_size, size_t max_record_count, bool output_to_console);
    virtual ~LazyLog();

protected:
    virtual void save_record(LOG_LEVEL log_level, const char * data, size_t size);

private:
    void flush();

private:
    struct LOG_RECORD
    {
        char   data[LOG_RECORD_SIZE];
        size_t size;
    };

private:
    LOG_RECORD     * m_record;
    size_t           m_reserve;
    size_t           m_begin;
    size_t           m_count;
    ThreadLocker     m_record_locker;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_LAZY_LOG_H
