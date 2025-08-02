/********************************************************
 * Description : make sure program only runs an entity
 * Data        : 2015-07-02 10:55:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <errno.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <cstdio>
    #include <cstdlib>
#endif // _MSC_VER

#include <string>
#include <sstream>

#include "base/utility/exclusive.h"
#include "base/log/log.h"

static bool exclusive_init(const char * exclusive_unique_name, size_t & unique_id, bool global_exclusive)
{
    if (nullptr == exclusive_unique_name)
    {
        RUN_LOG_ERR("exclusive_unique_name is nullptr");
        return false;
    }

#ifdef _MSC_VER
    unique_id = reinterpret_cast<size_t>(nullptr);

    std::string exclusive_file;
    {
        std::ostringstream oss;
        if (global_exclusive)
        {
            oss << "Global\\mutex_" << exclusive_unique_name;
        }
        else
        {
            oss << "mutex_" << exclusive_unique_name;
        }
        exclusive_file = oss.str();
    }

    HANDLE mutex = ::CreateMutex(nullptr, FALSE, exclusive_file.c_str());
    if (ERROR_ALREADY_EXISTS == stupid_system_error() || ERROR_ACCESS_DENIED == stupid_system_error())
    {
        RUN_LOG_ERR("another process has started");
        if (nullptr != mutex)
        {
            ::CloseHandle(mutex);
        }
        return false;
    }
    else if (nullptr == mutex)
    {
        RUN_LOG_ERR("create mutex %s failed: %d", exclusive_file.c_str(), stupid_system_error());
        return false;
    }

    unique_id = reinterpret_cast<size_t>(mutex);
#else
    unique_id = static_cast<size_t>(-1);

    std::string exclusive_file;
    {
        std::ostringstream oss;
        if (global_exclusive)
        {
            oss << "/var/run/" << exclusive_unique_name << ".pid";
        }
        else
        {
            const char * user_path = getenv("HOME");
            if (nullptr != user_path && '\0' != user_path[0])
            {
                oss << user_path << "/" << exclusive_unique_name << ".pid";
            }
            else
            {
                oss << "./" << exclusive_unique_name << ".pid";
            }
        }
        exclusive_file = oss.str();
    }

    int fd = ::open(exclusive_file.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {
        RUN_LOG_ERR("open %s failed: %d", exclusive_file.c_str(), stupid_system_error());
        return false;
    }

    struct flock locker;
    locker.l_type = F_WRLCK;
    locker.l_start = 0;
    locker.l_whence = SEEK_SET;
    locker.l_len = 0;
    if (::fcntl(fd, F_SETLK, &locker) < 0)
    {
        if (EACCES == stupid_system_error() || EAGAIN == stupid_system_error() || EDEADLK == stupid_system_error())
        {
            RUN_LOG_ERR("another process has started");
        }
        else
        {
            RUN_LOG_ERR("lock %s failed: %d", exclusive_file.c_str(), stupid_system_error());
        }
        ::close(fd);
        return false;
    }

    std::string pid;
    {
        std::ostringstream oss;
        oss << static_cast<size_t>(getpid());
        pid = oss.str();
    }

    if (::ftruncate(fd, 0) < 0)
    {
        RUN_LOG_ERR("truncate %s failed: %d", exclusive_file.c_str(), stupid_system_error());
    }

    if (::write(fd, pid.c_str(), pid.size()) < 0)
    {
        RUN_LOG_ERR("write %s failed: %d", exclusive_file.c_str(), stupid_system_error());
    }

    unique_id = static_cast<size_t>(fd);
#endif // _MSC_VER

    return true;
}

static void exclusive_exit(size_t & unique_id)
{
#ifdef _MSC_VER
    HANDLE mutex = reinterpret_cast<HANDLE>(unique_id);
    if (nullptr != mutex)
    {
        ::CloseHandle(mutex);
    }
    unique_id = reinterpret_cast<size_t>(nullptr);
#else
    int fd = static_cast<int>(unique_id);
    if (fd >= 0)
    {
        ::close(fd);
    }
    unique_id = static_cast<size_t>(-1);
#endif // _MSC_VER
}

static bool exclusive_is_exist(const char * exclusive_unique_name, bool global_exclusive)
{
#ifdef _MSC_VER
    std::string exclusive_file;
    {
        std::ostringstream oss;
        if (global_exclusive)
        {
            oss << "Global\\mutex_" << exclusive_unique_name;
        }
        else
        {
            oss << "mutex_" << exclusive_unique_name;
        }
        exclusive_file = oss.str();
    }

    HANDLE mutex = ::CreateMutexA(nullptr, FALSE, exclusive_file.c_str());
    bool is_exist = (ERROR_ALREADY_EXISTS == stupid_system_error() || ERROR_ACCESS_DENIED == stupid_system_error());
    if (nullptr != mutex)
    {
        ::CloseHandle(mutex);
    }

    return is_exist;
#else
    std::string exclusive_file;
    {
        std::ostringstream oss;
        if (global_exclusive)
        {
            oss << "/var/run/" << exclusive_unique_name << ".pid";
        }
        else
        {
            const char * user_path = getenv("HOME");
            if (nullptr != user_path && '\0' != user_path[0])
            {
                oss << user_path << "/" << exclusive_unique_name << ".pid";
            }
            else
            {
                oss << "./" << exclusive_unique_name << ".pid";
            }
        }
        exclusive_file = oss.str();
    }

    int fd = ::open(exclusive_file.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {
        return false;
    }

    bool is_exist = false;
    struct flock locker;
    locker.l_type = F_WRLCK;
    locker.l_start = 0;
    locker.l_whence = SEEK_SET;
    locker.l_len = 0;
    if (::fcntl(fd, F_SETLK, &locker) < 0)
    {
        is_exist = (EACCES == stupid_system_error() || EAGAIN == stupid_system_error() || EDEADLK == stupid_system_error());
    }

    ::close(fd);

    return is_exist;
#endif // _MSC_VER
}

NAMESPACE_STUPID_BASE_BEGIN

MutexApplication::MutexApplication()
    : m_exclusive(false)
    , m_unique_id(0)
{

}

MutexApplication::~MutexApplication()
{
    if (m_exclusive)
    {
        exclusive_exit(m_unique_id);
        m_exclusive = false;
    }
}

bool MutexApplication::exclusive(const char * exclusive_unique_name, bool global_exclusive)
{
    if (!m_exclusive)
    {
        m_exclusive = exclusive_init(exclusive_unique_name, m_unique_id, global_exclusive);
    }
    return m_exclusive;
}

bool mutex_application_is_exist(const char * exclusive_unique_name, bool global_exclusive)
{
    return exclusive_is_exist(exclusive_unique_name, global_exclusive);
}

NAMESPACE_STUPID_BASE_END
