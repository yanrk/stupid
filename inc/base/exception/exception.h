/********************************************************
 * Description : exception class
 * Data        : 2013-05-20 06:56:52
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_EXCEPTION_H
#define STUPID_BASE_EXCEPTION_H


#include <string>
#include "base/common/common.h"
#include "base/string/string.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API Exception
{
public:
    Exception(const char * file, int line, const char * func, const char * msg);

public:
    const std::string & what() const;

protected:
    std::string   m_message;
};


#define BASE_THROW(fmt, ...)                                              \
do                                                                        \
{                                                                         \
    const size_t MAX_EXCEPTION_MSG_LENGTH = 1024;                         \
    char msg[MAX_EXCEPTION_MSG_LENGTH] = { 0 };                           \
    stupid_snprintf(msg, MAX_EXCEPTION_MSG_LENGTH, fmt, ##__VA_ARGS__);   \
    throw Exception(__FILE__, __LINE__, __FUNCTION__, msg);               \
} while (false)

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_EXCEPTION_H
