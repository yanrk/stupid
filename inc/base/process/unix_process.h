/********************************************************
 * Description : process class in unix
 * Data        : 2016-09-09 15:20:14
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_UNIX_PROCESS_H
#define STUPID_BASE_UNIX_PROCESS_H


#ifndef _MSC_VER


#include <pthread.h>

#include <string>
#include <vector>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/locker/locker.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API UnixJoinProcess : private Uncopy
{
public:
    explicit UnixJoinProcess(const char * name = nullptr);
    explicit UnixJoinProcess(const std::string & command_line, const char * name = nullptr);
    explicit UnixJoinProcess(const std::vector<std::string> & command_line_params, const char * name = nullptr);
    ~UnixJoinProcess();

public:
    bool acquire();
    void release(bool process_tree = false, int exit_code = 9);
    bool wait_exit(int & exit_code);

public:
    void set_process_args(const std::string & command_line, const char * name = nullptr);
    void set_process_args(const std::vector<std::string> & command_line_params, const char * name = nullptr);

public:
    bool running();
    std::string process_name();
    size_t process_id() const;

private:
    void clear();

private:
    std::string                 m_name;
    pid_t                       m_pid;
    volatile bool               m_running;
    ThreadLocker                m_locker;
    std::vector<std::string>    m_command_line_params;
};

STUPID_CXX_API(bool) stupid_create_detached_process(const std::string & command_line);
STUPID_CXX_API(bool) stupid_create_detached_process(const std::vector<std::string> & command_line_params);

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER


#endif // STUPID_BASE_UNIX_PROCESS_H
