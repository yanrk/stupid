/********************************************************
 * Description : library dynamic import functions
 * Data        : 2015-03-24 09:22:00
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_LIBRARY_H
#define STUPID_BASE_LIBRARY_H


#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

STUPID_C_API(void *) stupid_library_acquire(const char * filename); /* null: failure */
STUPID_C_API(int) stupid_library_release(void * library); /* 1: success, 0: failure */
STUPID_C_API(void *) stupid_library_symbol(void * library, const char * symbol); /* null: failure */
STUPID_C_API(const char *) stupid_library_error(void); /* "": success */

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_LIBRARY_H
