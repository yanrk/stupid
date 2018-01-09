/********************************************************
 * Description : file operate class
 * Data        : 2015-08-24 17:21:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_FILE_H
#define STUPID_BASE_FILE_H


#ifdef _MSC_VER
    #include <wtypes.h>
#endif // _MSC_VER

#include <cstdint>
#include <cstdio>
#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/filesystem/sys_io.h"

NAMESPACE_STUPID_BASE_BEGIN

struct SeekPosition
{
    enum { SEEK_POS_BEG, SEEK_POS_CUR, SEEK_POS_END };
};

class STUPID_API File : private Uncopy
{
public:
    File();
    ~File();

public:
    bool open(const char * filename, bool is_write, bool is_truncate);
    bool is_open() const;
    bool is_eof() const;
    bool read(char * buff, size_t buff_size, size_t & read_len);    /* if (m_is_write) return(false); */
    bool write(const char * data, size_t data_len);                 /* if (!m_is_write) return(false); */
    bool seek(int64_t offset, int whence);                          /* if (m_is_write && !m_is_truncate) return(false); */
    bool seek(int64_t file_pos);                                    /* if (m_is_write && !m_is_truncate) return(false); */
    bool tell(int64_t & file_pos);                                  /* if (m_is_write && !m_is_truncate) return(false); */
    bool flush();
    void close();

private:
    bool                m_is_write;
    bool                m_is_truncate;
    std::string         m_file_name;
#ifdef _MSC_VER
    HANDLE              m_file;
    bool                m_is_eof;
#else
    FILE              * m_file;
#endif // _MSC_VER
};

STUPID_CXX_API(FILE *) file_open_for_read(const char * filename);
STUPID_CXX_API(FILE *) file_open_for_write(const char * filename, bool is_truncate);
STUPID_CXX_API(void) file_close(FILE *& file);
STUPID_CXX_API(bool) file_is_open(const FILE * file);
STUPID_CXX_API(bool) file_eof(FILE * file);
STUPID_CXX_API(bool) file_write(FILE * file, const char * data, size_t data_len);
STUPID_CXX_API(bool) file_read(FILE * file, char * buff, size_t buff_size, size_t & read_len);
STUPID_CXX_API(bool) file_seek(FILE * file, int64_t offset, int whence);
STUPID_CXX_API(bool) file_seek(FILE * file, int64_t file_pos);
STUPID_CXX_API(bool) file_tell(FILE * file, int64_t & file_pos);
STUPID_CXX_API(bool) file_flush(FILE * file);
STUPID_CXX_API(bool) file_get_size(const char * filename, int64_t & filesize);
STUPID_CXX_API(bool) file_get_time(const char * filename, int64_t & filetime);
STUPID_CXX_API(bool) file_set_time(const char * filename, int64_t filetime);

STUPID_CXX_API(bool) stupid_path_is_directory(const char * path_name, bool & path_is_directory);
STUPID_CXX_API(bool) stupid_mkdir_safe(const char * dir_name);
STUPID_CXX_API(bool) stupid_rmdir_safe(const char * dir_name);
STUPID_CXX_API(bool) stupid_access_safe(const char * path_name);
STUPID_CXX_API(bool) stupid_stat_safe(const char * path_name, stupid_stat_t & path_stat);
STUPID_CXX_API(bool) stupid_rename_safe(const char * src_path_name, const char * dst_path_name);
STUPID_CXX_API(bool) stupid_unlink_safe(const char * file_name);
STUPID_CXX_API(bool) stupid_chmod_safe(const char * file_name);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_FILE_H
