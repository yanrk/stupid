/********************************************************
 * Description : process class in windows
 * Data        : 2016-09-09 16:38:54
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_WINDOWS_PROCESS_H
#define STUPID_BASE_WINDOWS_PROCESS_H


#ifdef _MSC_VER


#include <windows.h>

#include <string>
#include <vector>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/locker/locker.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API WindowsJoinProcess : private Uncopy
{
public:
    explicit WindowsJoinProcess(const char * name = nullptr);
    explicit WindowsJoinProcess(const std::string & command_line, const char * name = nullptr);
    explicit WindowsJoinProcess(const std::vector<std::string> & command_line_params, const char * name = nullptr);
    ~WindowsJoinProcess();

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
    DWORD                       m_pid;
    HANDLE                      m_handle;
    volatile bool               m_running;
    ThreadLocker                m_locker;
    std::vector<std::string>    m_command_line_params;
};

STUPID_CXX_API(bool) stupid_create_detached_process(const std::string & command_line);
STUPID_CXX_API(bool) stupid_create_detached_process(const std::vector<std::string> & command_line_params);

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER


#endif // STUPID_BASE_WINDOWS_PROCESS_H
