/********************************************************
 * Description : typedefs of stupid base
 * Data        : 2013-05-18 17:43:12
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_TYPEDEFS_H
#define STUPID_BASE_TYPEDEFS_H


#include "base/common/macros.h"

#ifdef _MSC_VER
    typedef unsigned int         thread_return_t;
    typedef void *               thread_argument_t;
    #define THREAD_DEFAULT_RET   0U
#else
    typedef void *               thread_return_t;
    typedef void *               thread_argument_t;
    #define THREAD_DEFAULT_RET   nullptr
#endif // _MSC_VER

typedef thread_return_t (STUPID_STDCALL * thread_func_ptr_t) (thread_argument_t);


#endif // STUPID_BASE_TYPEDEFS_H
