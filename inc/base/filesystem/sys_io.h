/********************************************************
 * Description : macros of system standard I/O
 * Data        : 2013-05-23 11:46:45
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
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
    #define STUPID_R_OK     4
    #define STUPID_W_OK     2
    #define STUPID_X_OK     1
    #define STUPID_F_OK     0
#else
    #define STUPID_R_OK     R_OK
    #define STUPID_W_OK     W_OK
    #define STUPID_X_OK     X_OK
    #define STUPID_F_OK     F_OK
#endif // _MSC_VER

#ifdef _MSC_VER
    #define stupid_umask()            (0)
    #define stupid_chmod(file)        ::chmod(file, S_IREAD | S_IWRITE)
    #define stupid_open(file, mode)   ::_open(file, mode, S_IREAD | S_IWRITE)
    #define stupid_close              ::_close
    #define stupid_lseek              ::_lseek
    #define stupid_read               ::_read
    #define stupid_write              ::_write
    #define stupid_stat               ::_stat64
    #define stupid_stat_t             struct _stat64
    #define stupid_access             ::_access
    #define stupid_mkdir(dir)         ::_mkdir(dir)
    #define stupid_rmdir(dir)         ::_rmdir(dir)
    #define stupid_getcwd(dir, len)   ::_getcwd(dir, len)
    #define stupid_setcwd(dir)        ::_chdir(dir)
    #define stupid_rename             ::rename
    #define stupid_unlink             ::_unlink
    #define stupid_sync               ::_commit
#else
    #define stupid_umask()            ::umask(S_IWGRP | S_IWOTH)
    #define stupid_chmod(file)        ::chmod(file, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
    #define stupid_open(file, mode)   ::open(file, mode, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
    #define stupid_close              ::close
    #define stupid_lseek              ::lseek
    #define stupid_read               ::read
    #define stupid_write              ::write
#ifdef _MAC_OS
    #define stupid_stat               ::stat
    #define stupid_stat_t             struct stat
#else
    #define stupid_stat               ::stat64
    #define stupid_stat_t             struct stat64
#endif // _MAC_OS
    #define stupid_access             ::access
    #define stupid_mkdir(dir)         ::mkdir(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
    #define stupid_rmdir(dir)         ::rmdir(dir)
    #define stupid_getcwd(dir, len)   ::getcwd(dir, len)
    #define stupid_setcwd(dir)        ::chdir(dir)
    #define stupid_rename             ::rename
    #define stupid_unlink             ::unlink
    #define stupid_sync               ::fsync
#endif // _MSC_VER


#endif // STUPID_BASE_SYSTEM_IO_H
