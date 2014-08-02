/********************************************************
 * Description : macros of system standard I/O
 * Data        : 2013-05-23 11:46:45
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_SYSTEM_IO_H
#define STUPID_BASE_SYSTEM_IO_H


#ifdef _MSC_VER
    #include <io.h>
    #include <direct.h>
#else
    #include <unistd.h>
#endif // _MSC_VER

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _MSC_VER
    #define stupid_umask()            (0)
    #define stupid_open(file, mode)   ::_open(file, mode, S_IREAD | S_IWRITE)
    #define stupid_close              ::_close
    #define stupid_lseek              ::_lseek
    #define stupid_read               ::_read
    #define stupid_write              ::_write
    #define stupid_stat               ::_stat
    #define STUPID_STAT               struct _stat
    #define stupid_access             ::_access
    #define stupid_mkdir(dir)         ::_mkdir(dir)
    #define stupid_rmdir(dir)         ::_rmdir(dir)
    #define stupid_rename             ::rename
    #define stupid_unlink             ::_unlink
    #define stupid_sync               ::_commit
#else
    #define stupid_umask()            ::umask(S_IWGRP | S_IWOTH)
    #define stupid_open(file, mode)   ::open(file, mode, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
    #define stupid_close              ::close
    #define stupid_lseek              ::lseek
    #define stupid_read               ::read
    #define stupid_write              ::write
    #define stupid_stat               ::stat
    #define STUPID_STAT               struct stat
    #define stupid_access             ::access
    #define stupid_mkdir(dir)         ::mkdir(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
    #define stupid_rmdir(dir)         ::rmdir(dir)
    #define stupid_rename             ::rename
    #define stupid_unlink             ::unlink
    #define stupid_sync               ::fsync
#endif // _MSC_VER


#endif // STUPID_BASE_SYSTEM_IO_H
