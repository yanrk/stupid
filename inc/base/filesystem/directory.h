/********************************************************
 * Description : directory traversal class
 * Data        : 2013-06-06 14:46:16
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_DIRECTORY_H
#define STUPID_BASE_DIRECTORY_H


#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <dirent.h>
    #include <sys/stat.h>
#endif // _MSC_VER

#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API Directory : private Uncopy
{
public:
    Directory();
    ~Directory();

public:
    bool open(const char * dirname);
    bool is_open() const;
    bool read(); /* this function will ignore return-directory "." and ".." while traversing */
    void close();

public:
    const std::string & sub_file_name() const;
    const std::string & sub_file_short_name() const;
    bool sub_file_is_dir() const;

private:
    std::string          m_dir_name;
    std::string          m_current_sub_file_name;
    std::string          m_current_sub_file_short_name;
    bool                 m_current_sub_file_is_dir;

#ifdef _MSC_VER
    HANDLE               m_dir;
    WIN32_FIND_DATAA     m_file;
    bool                 m_eof;
#else
    DIR                * m_dir;
    struct dirent      * m_file;
#endif // _MSC_VER
};

STUPID_CXX_API(void) stupid_create_directory_recursive(const std::string & dirname);
STUPID_CXX_API(void) stupid_remove_directory_recursive(const std::string & dirname);

STUPID_CXX_API(void) stupid_directory_format_to_windows(std::string & dirname);
STUPID_CXX_API(void) stupid_directory_format_to_unix(std::string & dirname);
STUPID_CXX_API(void) stupid_directory_format_to_platform(std::string & dirname);

STUPID_CXX_API(bool) stupid_get_current_work_directory(std::string & dirname);
STUPID_CXX_API(bool) stupid_set_current_work_directory(const std::string & dirname);

STUPID_CXX_API(bool) stupid_extract_directory(const char * filename, std::string & dirname, bool format = false);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_DIRECTORY_H
