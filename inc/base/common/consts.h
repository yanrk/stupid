/********************************************************
 * Description : constants of stupid base
 * Data        : 2014-01-06 10:29:46
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_CONSTS_H
#define STUPID_BASE_CONSTS_H


#include "base/common/macros.h"
#include "base/common/namespace.h"

NAMESPACE_STUPID_BASE_BEGIN

#ifdef _MSC_VER
    #define g_directory_separator   '\\'
#else
    #define g_directory_separator   '/'
#endif // _MSC_VER

#define g_blank_character_set       " \t\v\r\n\f"

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_CONSTS_H
