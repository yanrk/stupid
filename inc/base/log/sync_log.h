/********************************************************
 * Description : log class of sync mode
 * Data        : 2013-05-23 14:02:13
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_SYNC_LOG_H
#define STUPID_BASE_SYNC_LOG_H


#include "base/common/common.h"
#include "base/log/log_base.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API SyncLog : public LogBase
{
public:
    SyncLog(const std::string & path, const std::string & log_type, LOG_LEVEL min_log_level, size_t max_file_size, bool output_to_console);
    virtual ~SyncLog();

protected:
    virtual void save_record(LOG_LEVEL log_level, const char * data, size_t size);
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_SYNC_LOG_H
