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

#ifdef _MSC_VER


#include <windows.h>
#include <tlhelp32.h>
#include "base/process/windows_process.h"
#include "base/string/string.h"
#include "base/utility/guard.h"

struct process_info_t
{
    DWORD   pid;
    DWORD   ppid;
};

static void get_all_process(std::list<process_info_t> & process_info_list)
{
    process_info_list.clear();

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == snapshot)
    {
        return;
    }

    PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };

    for (BOOL ok = Process32First(snapshot, &pe); TRUE == ok; ok = Process32Next(snapshot, &pe))
    {
        process_info_t process_info;
        process_info.pid = pe.th32ProcessID;
        process_info.ppid = pe.th32ParentProcessID;
        process_info_list.push_back(process_info);
    }

    CloseHandle(snapshot);
}

static void kill_process(DWORD process_id, int exit_code)
{
    if (0 == process_id)
    {
        return;
    }

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (nullptr == process)
    {
        return;
    }

    UINT exit_status = static_cast<UINT>(exit_code);
    TerminateProcess(process, exit_status);
}

static void kill_process_tree(DWORD process_id, int exit_code)
{
    if (0 == process_id)
    {
        return;
    }

    std::list<process_info_t> process_info_list;
    get_all_process(process_info_list);

    std::list<DWORD> process_tree;
    process_tree.push_back(process_id);

    while (true)
    {
        bool find_child_process = false;

        std::list<process_info_t>::iterator iter = process_info_list.begin();
        while (process_info_list.end() != iter)
        {
            if (process_tree.end() != std::find(process_tree.begin(), process_tree.end(), iter->ppid))
            {
                process_tree.push_back(iter->pid);
                iter = process_info_list.erase(iter);
                find_child_process = true;
            }
            else
            {
                ++iter;
            }
        }

        if (!find_child_process)
        {
            break;
        }
    }

    for (std::list<DWORD>::reverse_iterator iter = process_tree.rbegin(); process_tree.rend() != iter; ++iter)
    {
        kill_process(*iter, exit_code);
    }
}

static void command_params_to_command_line(const std::vector<std::string> & command_params, std::string & command_line)
{
    command_line.clear();
    for (std::vector<std::string>::const_iterator iter = command_params.begin(); command_params.end() != iter; ++iter)
    {
        const std::string & param = *iter;
        if (command_params.begin() != iter)
        {
            command_line += " ";
        }
        if ((param.empty()) || ('\"' != param[0] && std::string::npos != param.find(' ')))
        {
            command_line += '\"' + param + '\"';
        }
        else
        {
            command_line += param;
        }
    }
}

NAMESPACE_STUPID_BASE_BEGIN

WindowsJoinProcess::WindowsJoinProcess(const char * name)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_handle(nullptr)
    , m_running(false)
    , m_locker("thread locker of sub process")
    , m_command_line_params()
{

}

WindowsJoinProcess::WindowsJoinProcess(const std::string & command_line, const char * name)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_handle(nullptr)
    , m_running(false)
    , m_locker("thread locker of sub process")
    , m_command_line_params()
{
    stupid_split_command_line(command_line.c_str(), m_command_line_params);
}

WindowsJoinProcess::WindowsJoinProcess(const std::vector<std::string> & command_line_params, const char * name)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_handle(nullptr)
    , m_running(false)
    , m_locker("thread locker of sub process")
    , m_command_line_params(command_line_params)
{

}

WindowsJoinProcess::~WindowsJoinProcess()
{
    release();
    clear();
}

bool WindowsJoinProcess::acquire()
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (m_running || m_command_line_params.empty())
    {
        return false;
    }
    m_running = true;

    std::string command_line;
    command_params_to_command_line(m_command_line_params, command_line);

    clear();

    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi = { 0x00 };

    if (!CreateProcessA(nullptr, reinterpret_cast<LPSTR>(const_cast<char *>(command_line.c_str())), nullptr, nullptr, false, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi))
    {
        m_running = false;
        return false;
    }
    CloseHandle(pi.hThread);

    m_pid = pi.dwProcessId;
    m_handle = pi.hProcess;

    return true;
}

void WindowsJoinProcess::release(bool process_tree, int exit_code)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running || 0 == m_pid)
    {
        return;
    }
    m_running = false;

    if (process_tree)
    {
        kill_process_tree(m_pid, exit_code);
    }
    else
    {
        kill_process(m_pid, exit_code);
    }
}

bool WindowsJoinProcess::wait_exit(int & exit_code)
{
    if (!m_running || nullptr == m_handle)
    {
        return false;
    }

    bool ret = false;

    const DWORD pid = m_pid;

    do
    {
        exit_code = -1;

        if (WAIT_OBJECT_0 != WaitForSingleObject(m_handle, INFINITE))
        {
            break;
        }

        DWORD exit_status = ~static_cast<DWORD>(0);
        if (!GetExitCodeProcess(m_handle, &exit_status))
        {
            break;
        }

        /*
        if (STILL_ACTIVE == exit_status)
        {
            break;
        }
        */

        exit_code = static_cast<int>(exit_status);

        ret = true;
    } while (false);

    if (pid == m_pid)
    {
        clear();

        m_running = false;
    }
    else
    {
        ret = false;
    }

    return ret;
}

void WindowsJoinProcess::clear()
{
    if (nullptr != m_handle)
    {
        CloseHandle(m_handle);
    }
    m_pid = 0;
    m_handle = nullptr;
}

void WindowsJoinProcess::set_process_args(const std::string & command_line, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_name = (nullptr != name ? name : "");
        m_command_line_params.clear();
        stupid_split_command_line(command_line.c_str(), m_command_line_params);
    }
}

void WindowsJoinProcess::set_process_args(const std::vector<std::string> & command_line_params, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_name = (nullptr != name ? name : "");
        m_command_line_params = command_line_params;
    }
}

bool WindowsJoinProcess::running()
{
    return m_running;
}

std::string WindowsJoinProcess::process_name()
{
    return m_name;
}

size_t WindowsJoinProcess::process_id() const
{
    return static_cast<size_t>(m_pid);
}

bool stupid_create_detached_process(const std::string & command_line)
{
    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi = { 0x00 };

    if (!CreateProcessA(nullptr, reinterpret_cast<LPSTR>(const_cast<char *>(command_line.c_str())), nullptr, nullptr, false, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi))
    {
        return false;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return true;
}

bool stupid_create_detached_process(const std::vector<std::string> & command_line_params)
{
    std::string command_line;
    command_params_to_command_line(command_line_params, command_line);
    return stupid_create_detached_process(command_line);
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER
