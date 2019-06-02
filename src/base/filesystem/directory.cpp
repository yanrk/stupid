/********************************************************
 * Description : directory traversal class
 * Data        : 2013-06-06 14:46:16
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cstring>
#include <list>
#include <string>
#include <algorithm>
#include "base/string/string.h"
#include "base/charset/charset.h"
#include "base/filesystem/sys_io.h"
#include "base/filesystem/file.h"
#include "base/filesystem/directory.h"

NAMESPACE_STUPID_BASE_BEGIN

Directory::Directory()
    : m_dir_name()
    , m_current_sub_path_name()
    , m_current_sub_path_short_name()
    , m_current_sub_path_is_directory(false)
#ifdef _MSC_VER
    , m_dir(INVALID_HANDLE_VALUE)
    , m_file()
    , m_eof(true)
#else
    , m_dir(nullptr)
    , m_file(nullptr)
#endif // _MSC_VER
{

}

Directory::~Directory()
{
    close();
}

bool Directory::open(const char * dirname)
{
    close();

    if (nullptr == dirname || '\0' == dirname[0])
    {
        return(false);
    }

    m_dir_name = dirname;
    if ('/' != *m_dir_name.rbegin() && '\\' != *m_dir_name.rbegin())
    {
        m_dir_name += g_directory_separator;
    }

#ifdef _MSC_VER
    std::wstring pattern(utf8_to_unicode(m_dir_name) + L"*");
    m_dir = FindFirstFileW(pattern.c_str(), &m_file);
    if (INVALID_HANDLE_VALUE == m_dir)
    {
        return(false);
    }
    m_eof = false;
#else
    m_dir = opendir(utf8_to_ansi(m_dir_name).c_str());
    if (nullptr == m_dir)
    {
        return(false);
    }
    m_file = readdir(m_dir);
#endif // _MSC_VER

    return(true);
}

bool Directory::is_open() const
{
#ifdef _MSC_VER
    return(INVALID_HANDLE_VALUE != m_dir);
#else
    return(nullptr != m_dir);
#endif // _MSC_VER
}

bool Directory::read()
{
    if (!is_open())
    {
        return(false);
    }

#ifdef _MSC_VER
    while (!m_eof)
    {
        const std::wstring file_name(m_file.cFileName);
        const DWORD file_type = m_file.dwFileAttributes;
        m_eof = !FindNextFileW(m_dir, &m_file);

        if (L"." == file_name || L".." == file_name)
        {
            continue;
        }

        if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & file_type))
        {
            m_current_sub_path_short_name = unicode_to_utf8(file_name);
            m_current_sub_path_name = m_dir_name + m_current_sub_path_short_name + g_directory_separator;
            m_current_sub_path_is_directory = true;
        }
        else
        {
            m_current_sub_path_short_name = unicode_to_utf8(file_name);
            m_current_sub_path_name = m_dir_name + m_current_sub_path_short_name;
            m_current_sub_path_is_directory = false;
        }

        return(true);
    }
#else
    while (nullptr != m_file)
    {
        /*
         * do not do like this:
         *     struct dirent * file = m_file;
         *     m_file = readdir(m_dir);
         *     operate_function(file);
         * the behavior is undefined, the result is not expected
         */
        const std::string d_name(m_file->d_name);
#if 0
        const size_t d_type = m_file->d_type;
#endif // 0
        m_file = readdir(m_dir);

        if ("." == d_name || ".." == d_name)
        {
            continue;
        }

#if 0
        /*
         * d_type: not supported by all filesystem
         */
        if (DT_DIR == (DT_DIR & d_type))
        {
            m_current_sub_path_short_name = ansi_to_utf8(d_name);
            m_current_sub_path_name = m_dir_name + m_current_sub_path_short_name + g_directory_separator;
            m_current_sub_path_is_directory = true;
            return(true);
        }
        else if (DT_REG == (DT_REG & d_type))
        {
            m_current_sub_path_short_name = ansi_to_utf8(d_name);
            m_current_sub_path_name = m_dir_name + m_current_sub_path_short_name;
            m_current_sub_path_is_directory = false;
            return(true);
        }
#else
        stupid_stat_t stat_buf = { 0x00 };
        const std::string file_name(utf8_to_ansi(m_dir_name) + d_name);
        if (0 != stupid_stat(file_name.c_str(), &stat_buf))
        {
            continue;
        }

        if (S_IFDIR == (S_IFDIR & stat_buf.st_mode))
        {
            m_current_sub_path_short_name = ansi_to_utf8(d_name);
            m_current_sub_path_name = m_dir_name + m_current_sub_path_short_name + g_directory_separator;
            m_current_sub_path_is_directory = true;
            return(true);
        }
        else if (S_IFREG == (S_IFREG & stat_buf.st_mode))
        {
            m_current_sub_path_short_name = ansi_to_utf8(d_name);
            m_current_sub_path_name = m_dir_name + m_current_sub_path_short_name;
            m_current_sub_path_is_directory = false;
            return(true);
        }
#endif // 0
    }
#endif // _MSC_VER

    m_current_sub_path_short_name.clear();
    m_current_sub_path_name.clear();
    m_current_sub_path_is_directory = false;

    return(false);
}

void Directory::close()
{
    if (!is_open())
    {
        return;
    }

    m_current_sub_path_short_name.clear();
    m_current_sub_path_name.clear();
    m_current_sub_path_is_directory = false;

#ifdef _MSC_VER
    FindClose(m_dir);
    m_dir = INVALID_HANDLE_VALUE;
#else
    closedir(m_dir);
    m_dir = nullptr;
#endif // _MSC_VER
}

const std::string & Directory::sub_path_name() const
{
    return(m_current_sub_path_name);
}

const std::string & Directory::sub_path_short_name() const
{
    return(m_current_sub_path_short_name);
}

bool Directory::sub_path_is_directory() const
{
    return(m_current_sub_path_is_directory);
}

void stupid_create_directory_recursive(const std::string & dirname)
{
    const std::string slashs("/\\");
    std::string::const_iterator iter_b = dirname.begin();
    std::string::const_iterator iter_e = dirname.end();
    std::string::const_iterator iter_f = iter_b;
    while (iter_f != iter_e)
    {
        iter_f = std::find_first_of(iter_f, iter_e, slashs.begin(), slashs.end());
        if (iter_f != iter_e)
        {
            ++iter_f;
        }
        const std::string fore_dir_name(iter_b, iter_f);
        stupid_mkdir_safe(fore_dir_name.c_str());
    }
}

void stupid_remove_directory_recursive(const std::string & dirname)
{
    if (dirname.empty())
    {
        return;
    }

    std::string dir_name(dirname);
    if ('/' != *dir_name.rbegin() && '\\' != *dir_name.rbegin())
    {
        dir_name += g_directory_separator;
    }

    std::list<std::string> sub_folder_list;
    std::list<std::string> folder_list;
    folder_list.push_back(dir_name);
    while (!folder_list.empty())
    {
        const std::string folder_name = folder_list.front();
        folder_list.pop_front();
        Directory dir;
        if (!dir.open(folder_name.c_str()))
        {
            continue;
        }
        while (dir.read())
        {
            if (dir.sub_path_is_directory())
            {
                sub_folder_list.push_front(dir.sub_path_name());
                folder_list.push_back(dir.sub_path_name());
            }
            else
            {
                stupid_chmod_safe(dir.sub_path_name().c_str());
                stupid_unlink_safe(dir.sub_path_name().c_str());
            }
        }
        dir.close();
    }
    for (std::list<std::string>::const_iterator iter = sub_folder_list.begin(); sub_folder_list.end() != iter; ++iter)
    {
        stupid_rmdir_safe(iter->c_str());
    }
    stupid_rmdir_safe(dir_name.c_str());
}

void stupid_directory_format_to_windows(std::string & dirname)
{
    std::replace(dirname.begin(), dirname.end(), '/', '\\');
}

void stupid_directory_format_to_unix(std::string & dirname)
{
    std::replace(dirname.begin(), dirname.end(), '\\', '/');
}

void stupid_directory_format(std::string & dirname)
{
#ifdef _MSC_VER
    stupid_directory_format_to_windows(dirname);
#else
    stupid_directory_format_to_unix(dirname);
#endif // _MSC_VER
}

void stupid_pathname_format_strictly_to_windows(const std::string & src_pathname, std::string & dst_pathname)
{
    dst_pathname.clear();

    if ((src_pathname.size() >= 2) && ('/' == src_pathname[0] || '\\' == src_pathname[0]) && ('/' == src_pathname[1] || '\\' == src_pathname[1]))
    {
        dst_pathname += "\\\\";
    }

    const char directory_separator = '\\';

    char last_character = directory_separator;
    for (std::string::const_iterator iter = src_pathname.begin(); src_pathname.end() != iter; ++iter)
    {
        if ('/' == *iter || '\\' == *iter)
        {
            if (directory_separator != last_character)
            {
                last_character = directory_separator;
                dst_pathname.push_back(last_character);
            }
        }
        else
        {
            last_character = *iter;
            dst_pathname.push_back(last_character);
        }
    }
}

std::string stupid_pathname_format_strictly_to_windows(const std::string & pathname)
{
    std::string pathname_strictly;
    stupid_pathname_format_strictly_to_windows(pathname, pathname_strictly);
    return(pathname_strictly);
}

void stupid_pathname_format_strictly_to_unix(const std::string & src_pathname, std::string & dst_pathname)
{
    dst_pathname.clear();

    if ((src_pathname.size() >= 1) && ('/' == src_pathname[0] || '\\' == src_pathname[0]))
    {
        dst_pathname += "/";
    }

    const char directory_separator = '/';

    char last_character = directory_separator;
    for (std::string::const_iterator iter = src_pathname.begin(); src_pathname.end() != iter; ++iter)
    {
        if ('/' == *iter || '\\' == *iter)
        {
            if (directory_separator != last_character)
            {
                last_character = directory_separator;
                dst_pathname.push_back(last_character);
            }
        }
        else
        {
            last_character = *iter;
            dst_pathname.push_back(last_character);
        }
    }
}

std::string stupid_pathname_format_strictly_to_unix(const std::string & pathname)
{
    std::string pathname_strictly;
    stupid_pathname_format_strictly_to_unix(pathname, pathname_strictly);
    return(pathname_strictly);
}

void stupid_pathname_format_strictly(const std::string & src_pathname, std::string & dst_pathname)
{
#ifdef _MSC_VER
    stupid_pathname_format_strictly_to_windows(src_pathname, dst_pathname);
#else
    stupid_pathname_format_strictly_to_unix(src_pathname, dst_pathname);
#endif // _MSC_VER
}

std::string stupid_pathname_format_strictly(const std::string & pathname)
{
    std::string pathname_strictly;
    stupid_pathname_format_strictly(pathname, pathname_strictly);
    return(pathname_strictly);
}

bool stupid_absolute_pathname_format_strictly(const std::string & src_pathname, std::string & dst_pathname, bool support_long_path)
{
    dst_pathname.clear();

    std::string::size_type start_pos = src_pathname.find_first_not_of(g_blank_character_set);
    if (std::string::npos == start_pos)
    {
        return(false);
    }
    bool start_with_directory_separator = ('/' == src_pathname[start_pos] || '\\' == src_pathname[start_pos]);

    std::list<std::string> src_path_node_list;
    stupid_split_piece(src_pathname, "\\/", false, src_path_node_list);

#ifdef _MSC_VER

    /*
     * valid format:
     *      mode 1: "c:\path"                           (c is a local drive)
     *      mode 2: "x:\path"                           (x is a network drive)
     *      mode 3: "\\server-machine\share-path"       (server-machine is a computer host name)
     *      mode 4: "\\192.168.0.1\share-path"          (192.168.0.1 is a computer ip)
     *      mode 5: "\\?\c:\path"                       (c is a local drive)
     *      mode 6: "\\?\x:\path"                       (x is a network drive)
     *      mode 7: "\\?\unc\server-machine\share-path" (server-machine is a computer host name)
     *      mode 8: "\\?\unc\192.168.0.1\share-path"    (192.168.0.1 is a computer ip)
     */

    if (src_path_node_list.empty())
    {
        return(false);
    }

    const std::string & first_path_node = src_path_node_list.front();
    if (start_with_directory_separator)
    {
        // mode: 3, 4, 5, 6, 7, 8
        if ("?" == first_path_node)
        {
            // mode: 5, 6, 7, 8
            src_path_node_list.pop_front();
            if (src_path_node_list.empty())
            {
                return(false);
            }

            const std::string & second_path_node = src_path_node_list.front();
            if (':' == *second_path_node.rbegin())
            {
                // mode: 5, 6
                dst_pathname = "\\\\?\\" + second_path_node;
                src_path_node_list.pop_front();
            }
            else if (0 == stupid_strcmp_ignore_case(second_path_node.c_str(), "UNC"))
            {
                // mode: 7, 8
                src_path_node_list.pop_front();
                if (src_path_node_list.empty())
                {
                    return(false);
                }

                const std::string & third_path_node = src_path_node_list.front();
                dst_pathname = "\\\\?\\UNC\\" + third_path_node;
                src_path_node_list.pop_front();
            }
            else
            {
                return(false);
            }
        }
        else
        {
            // mode: 3, 4
            if (support_long_path)
            {
                dst_pathname = "\\\\?\\UNC\\" + first_path_node;
            }
            else
            {
                dst_pathname = "\\\\" + first_path_node;
            }
            src_path_node_list.pop_front();
        }
    }
    else if (':' == *first_path_node.rbegin())
    {
        // mode: 1, 2
        if (support_long_path)
        {
            dst_pathname = "\\\\?\\" + first_path_node;
        }
        else
        {
            dst_pathname = first_path_node;
        }
        src_path_node_list.pop_front();
    }
    else
    {
        return(false);
    }

#else

    support_long_path = false; /* prevent warning */

    if (!start_with_directory_separator)
    {
        return(false);
    }

#endif // _MSC_VER

    std::list<std::string> dst_path_node_list;
    while (!src_path_node_list.empty())
    {
        const std::string & path_node = src_path_node_list.front();
        if ("." == path_node)
        {
            src_path_node_list.pop_front();
        }
        else if (".." == path_node)
        {
            if (!dst_path_node_list.empty())
            {
                dst_path_node_list.pop_back();
            }
            src_path_node_list.pop_front();
        }
        else
        {
            dst_path_node_list.push_back(path_node);
            src_path_node_list.pop_front();
        }
    }

    if (dst_path_node_list.empty())
    {
        dst_pathname += g_directory_separator;
    }
    else
    {
        while (!dst_path_node_list.empty())
        {
            dst_pathname += g_directory_separator + dst_path_node_list.front();
            dst_path_node_list.pop_front();
        }

        std::string::size_type end_pos = src_pathname.find_last_not_of(g_blank_character_set);
        if (std::string::npos != end_pos)
        {
            bool end_with_directory_separator = ('/' == src_pathname[end_pos] || '\\' == src_pathname[end_pos]);
            if (end_with_directory_separator)
            {
                dst_pathname += g_directory_separator;
            }
        }
    }

    return(true);
}

std::string stupid_absolute_pathname_format_strictly(const std::string & pathname, bool support_long_path)
{
    std::string pathname_strictly;
    stupid_absolute_pathname_format_strictly(pathname, pathname_strictly, support_long_path);
    return(pathname_strictly);
}

bool stupid_get_current_work_directory(std::string & dirname)
{
#ifdef _MSC_VER
    wchar_t temp[512] = { 0 };
    if (nullptr == _wgetcwd(temp, sizeof(temp) / sizeof(temp[0])))
    {
        dirname.clear();
        return(false);
    }
    else
    {
        dirname = unicode_to_utf8(std::wstring(temp)) + g_directory_separator;
        stupid_directory_format(dirname);
        return(true);
    }
#else
    char temp[512] = { 0 };
    if (nullptr == getcwd(temp, sizeof(temp) / sizeof(temp[0])))
    {
        dirname.clear();
        return(false);
    }
    else
    {
        dirname = ansi_to_utf8(std::string(temp)) + g_directory_separator;
        stupid_directory_format(dirname);
        return(true);
    }
#endif // _MSC_VER
}

bool stupid_set_current_work_directory(const std::string & dirname)
{
#ifdef _MSC_VER
    std::string platform_dirname(dirname);
    stupid_directory_format(platform_dirname);
    return(0 == _wchdir(utf8_to_unicode(platform_dirname).c_str()));
#else
    std::string platform_dirname(dirname);
    stupid_directory_format(platform_dirname);
    return(0 == chdir(utf8_to_ansi(platform_dirname).c_str()));
#endif // _MSC_VER
}

bool stupid_extract_directory(const char * pathname, std::string & dirname, bool format)
{
    if (nullptr == pathname || '\0' == pathname[0])
    {
        dirname.clear();
        return(false);
    }

    const char * pos_s = pathname;
    const char * lslash = strrchr(pos_s, '/');
    const char * rslash = strrchr(pos_s, '\\');
    const char * pos_e = std::max<const char *>(lslash, rslash);

    if (nullptr == pos_e)
    {
        std::string(pos_s).swap(dirname);
    }
    else
    {
        std::string(pos_s, pos_e).swap(dirname);
    }

    if (format)
    {
        dirname += g_directory_separator;
        stupid_directory_format(dirname);
    }

    return(true);
}

std::string stupid_extract_directory(const char * pathname, bool format)
{
    std::string dirname;
    stupid_extract_directory(pathname, dirname, format);
    return(dirname);
}

bool stupid_extract_file(const char * pathname, std::string & filename)
{
    if (nullptr == pathname || '\0' == pathname[0])
    {
        filename.clear();
        return(false);
    }

    const char * pos_s = pathname;
    const char * lslash = strrchr(pos_s, '/');
    const char * rslash = strrchr(pos_s, '\\');
    const char * pos_e = std::max<const char *>(lslash, rslash);

    if (nullptr == pos_e)
    {
        std::string(pos_s).swap(filename);
    }
    else
    {
        std::string(pos_e + 1).swap(filename);
    }

    return(true);
}

std::string stupid_extract_file(const char * pathname)
{
    std::string filename;
    stupid_extract_file(pathname, filename);
    return(filename);
}

NAMESPACE_STUPID_BASE_END
