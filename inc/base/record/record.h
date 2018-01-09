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

#ifndef STUPID_BASE_RECORD_H
#define STUPID_BASE_RECORD_H


#include <cstdint>
#include <string>
#include "base/common/common.h"
#include "base/locker/locker.h"
#include "base/filesystem/file.h"
#include "base/utility/uncopy.h"
#include "base/utility/singleton.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API Record : private Uncopy
{
public:
    bool init(const std::string & record_root_directory);
    void exit();

public:
    bool running() const;
    void write(const char * data, size_t size, bool success_record);

private:
    void write_success_record(const char * data, size_t size);
    void write_failure_record(const char * data, size_t size);

private:
    Record();
    ~Record();

private:
    friend class Singleton<Record>;

private:
    bool                    m_is_running;
    const int64_t           m_max_filesize;

private:
    std::string             m_success_dirname;
    std::string             m_success_filename;
    File                    m_success_file;
    int64_t                 m_success_filesize;
    ThreadLocker            m_success_file_locker;

private:
    std::string             m_failure_dirname;
    std::string             m_failure_filename;
    File                    m_failure_file;
    int64_t                 m_failure_filesize;
    ThreadLocker            m_failure_file_locker;
};

NAMESPACE_STUPID_BASE_END

STUPID_CXX_API(void) stupid_record(Stupid::Base::Record & record, bool success_record, const char * format, ...);

#define RECORD_SUCCESS(fmt, ...)    stupid_record(Stupid::Base::Singleton<Stupid::Base::Record>::instance(), true, fmt, ##__VA_ARGS__)

#define RECORD_FAILURE(fmt, ...)    stupid_record(Stupid::Base::Singleton<Stupid::Base::Record>::instance(), false, fmt, ##__VA_ARGS__)


#endif // STUPID_BASE_RECORD_H
