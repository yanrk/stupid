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

#ifndef _MSC_VER


#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include "base/process/unix_process.h"
#include "base/string/string.h"
#include "base/utility/guard.h"

struct process_info_t
{
    size_t  pid;
    size_t  ppid;
};

static void get_all_process(std::list<process_info_t> & process_info_list)
{
    process_info_list.clear();

    const char * command = "ps -eo pid,ppid";
    FILE * file = popen(command, "r");
    if (nullptr == file)
    {
        return;
    }

    const std::string spaces(g_blank_character_set);
    const size_t buff_size = 2048;
    char buff[buff_size] = { 0 };
    while (nullptr != fgets(buff, buff_size, file))
    {
        std::string line(buff);

        process_info_t process_info;

        std::string::size_type pid_b = line.find_first_not_of(spaces, 0);
        if (std::string::npos == pid_b)
        {
            continue;
        }
        std::string::size_type pid_e = line.find_first_of(spaces, pid_b);
        if (std::string::npos == pid_e)
        {
            continue;
        }
        std::string pid_value(line.begin() + pid_b, line.begin() + pid_e);
        if (!Stupid::Base::stupid_string_to_type(pid_value, process_info.pid))
        {
            continue;
        }

        std::string::size_type ppid_b = line.find_first_not_of(spaces, pid_e);
        if (std::string::npos == ppid_b)
        {
            continue;
        }
        std::string::size_type ppid_e = line.find_first_of(spaces, ppid_b);
        if (std::string::npos == ppid_e)
        {
            ppid_e = line.size();
        }
        std::string ppid_value(line.begin() + ppid_b, line.begin() + ppid_e);
        if (!Stupid::Base::stupid_string_to_type(ppid_value, process_info.ppid))
        {
            continue;
        }

        process_info_list.push_back(process_info);
    }

    pclose(file);
}

static void kill_process(size_t process_id, int exit_code)
{
    if (0 == process_id)
    {
        return;
    }

    kill(static_cast<pid_t>(process_id), exit_code);
}

static void kill_process_tree(size_t process_id, int exit_code)
{
    if (0 == process_id)
    {
        return;
    }

    std::list<process_info_t> process_info_list;
    get_all_process(process_info_list);

    std::list<size_t> process_tree;
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

    for (std::list<size_t>::reverse_iterator iter = process_tree.rbegin(); process_tree.rend() != iter; ++iter)
    {
        kill_process(*iter, exit_code);
    }
}

NAMESPACE_STUPID_BASE_BEGIN

UnixJoinProcess::UnixJoinProcess(const char * name)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_running(false)
    , m_locker("thread locker of sub process")
    , m_command_line_params()
{

}

UnixJoinProcess::UnixJoinProcess(const std::string & command_line, const char * name)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_running(false)
    , m_locker("thread locker of sub process")
    , m_command_line_params()
{
    stupid_split_command_line(command_line.c_str(), m_command_line_params);
}

UnixJoinProcess::UnixJoinProcess(const std::vector<std::string> & command_line_params, const char * name)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_running(false)
    , m_locker("thread locker of sub process")
    , m_command_line_params(command_line_params)
{

}

UnixJoinProcess::~UnixJoinProcess()
{
    release();
    clear();
}

bool UnixJoinProcess::acquire()
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (m_running || m_command_line_params.empty())
    {
        return(false);
    }
    m_running = true;

    clear();

    m_pid = fork();
    if (m_pid < 0)
    {
        m_running = false;
        return(false);
    }
    else if (0 == m_pid)
    {
        std::vector<const char *> argv(m_command_line_params.size() + 1, nullptr);
        for (size_t index = 0; index < m_command_line_params.size(); ++index)
        {
            argv[index] = m_command_line_params[index].c_str();
        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        /*
         * if execv() success, will not execute the next exit()
         */
        if (execv(argv[0], const_cast<char **>(&argv[0])) < 0)
        {
            exit(0); // should not be here
        }
        exit(0); // should never be here
    }

    return(true);
}

void UnixJoinProcess::release(bool process_tree, int exit_code)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running || 0 == m_pid)
    {
        return;
    }
    m_running = false;

    if (process_tree)
    {
        kill_process_tree(static_cast<size_t>(m_pid), exit_code);
    }
    else
    {
        kill_process(static_cast<size_t>(m_pid), exit_code);
    }
}

bool UnixJoinProcess::wait_exit(int & exit_code)
{
    if (!m_running || 0 == m_pid)
    {
        return(false);
    }

    bool ret = false;

    const pid_t pid = m_pid;

    do
    {
        exit_code = -1;

        int exit_status = -1;
        if (waitpid(m_pid, &exit_status, 0) != m_pid)
        {
            break;
        }

        if (WIFEXITED(exit_status))
        {
            exit_code = WEXITSTATUS(exit_status);
        }
        else if (WIFSIGNALED(exit_status))
        {
            exit_code = WTERMSIG(exit_status);
        }
        else if (WIFSTOPPED(exit_status))
        {
            exit_code = WSTOPSIG(exit_status);
        }
        else
        {
            /*
            if (WIFCONTINUED(exit_status))
            {
                break;
            }
            */
            break;
        }

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

    return(ret);
}

void UnixJoinProcess::clear()
{
    m_pid = 0;
}

void UnixJoinProcess::set_process_args(const std::string & command_line, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_name = (nullptr != name ? name : "");
        m_command_line_params.clear();
        stupid_split_command_line(command_line.c_str(), m_command_line_params);
    }
}

void UnixJoinProcess::set_process_args(const std::vector<std::string> & command_line_params, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_name = (nullptr != name ? name : "");
        m_command_line_params = command_line_params;
    }
}

bool UnixJoinProcess::running()
{
    return(m_running);
}

std::string UnixJoinProcess::process_name()
{
    return(m_name);
}

size_t UnixJoinProcess::process_id() const
{
    return(static_cast<size_t>(m_pid));
}

bool stupid_create_detached_process(const std::string & command_line)
{
    std::vector<std::string> command_line_params;
    stupid_split_command_line(command_line.c_str(), command_line_params);
    return(stupid_create_detached_process(command_line_params));
}

bool stupid_create_detached_process(const std::vector<std::string> & command_line_params)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        return(false);
    }
    else if (0 == pid)
    {
        pid_t cpid = fork();
        if (cpid < 0)
        {
            exit(0);
        }
        else if (0 == cpid)
        {
            std::vector<const char *> argv(command_line_params.size() + 1, nullptr);
            for (size_t index = 0; index < command_line_params.size(); ++index)
            {
                argv[index] = command_line_params[index].c_str();
            }
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            /*
             * if execv() success, will not execute the next exit()
             */
            if (execv(argv[0], const_cast<char **>(&argv[0])) < 0)
            {
                exit(0); // should not be here
            }
            exit(0); // should never be here
        }
        else
        {
            exit(0);
        }
    }
    else
    {
        int exit_status = -1;
        waitpid(pid, &exit_status, 0);
    }

    return(true);
}

NAMESPACE_STUPID_BASE_END


#endif // _MSC_VER
