/********************************************************
 * Description : type definition of log module
 * Data        : 2013-05-22 22:06:21
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_LOG_TYPES_H
#define STUPID_BASE_LOG_TYPES_H


#include <string>

enum STUPID_LOG_TYPE
{
    LOG_TYPE_MIN, 
    LOG_TYPE_RUN = LOG_TYPE_MIN, 
    LOG_TYPE_DBG, 
    LOG_TYPE_MAX  
};

const char * const STUPID_LOG_TYPE_INFO[LOG_TYPE_MAX] = 
{
    "run", "debug"
};

enum STUPID_LOG_WRITE_MODE
{
    SYNC_WRITE_MODE, 
    ASYN_WRITE_MODE, 
    LAZY_WRITE_MODE  
};

enum STUPID_LOG_LEVEL
{
    LOG_LEVEL_MIN, 
    CRI_LEVEL = LOG_LEVEL_MIN, 
    ERR_LEVEL, 
    WAR_LEVEL, 
    DBG_LEVEL, 
    TRK_LEVEL, 
    LOG_LEVEL_MAX
};

const char * const STUPID_LOG_LEVEL_INFO[LOG_LEVEL_MAX] = 
{
    "Critical", 
    "   Error", 
    " Warning", 
    "   Debug", 
    "   Track"  
};

struct STUPID_LOG_CONFIG
{
    std::string log_file_path;

    struct
    {
        STUPID_LOG_WRITE_MODE   write_mode;
        STUPID_LOG_LEVEL        log_min_level;
        size_t                  log_file_size;
        size_t                  buffer_count;
        bool                    output_to_console;
    } log_file[LOG_TYPE_MAX];
};

enum
{
    LOG_RECORD_SIZE = 2048
};


#endif // STUPID_BASE_LOG_TYPES_H
