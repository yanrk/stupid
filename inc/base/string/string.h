/********************************************************
 * Description : string operator functions
 * Data        : 2013-05-18 20:41:33
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_STRING_H
#define STUPID_BASE_STRING_H


#include <string>
#include <sstream>
#include <cstdarg>
#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

template <typename T>
bool stupid_string_to_type(const std::string & str, T & val)
{
    std::istringstream iss(str);
    iss.setf(std::ios::boolalpha);
    iss >> val;
    return(!iss.fail());
}

template <typename T>
bool stupid_type_to_string(T val, std::string & str)
{
    std::ostringstream oss;
    oss.setf(std::ios::fixed, std::ios::floatfield);
    oss.setf(std::ios::boolalpha);
    oss << val;
    str = oss.str();
    return(true);
}

STUPID_CXX_API(size_t) stupid_vsnprintf(char * buffer, size_t bufsiz, const char * fmt, va_list args);
STUPID_CXX_API(size_t) stupid_snprintf(char * buffer, size_t bufsiz, const char * fmt, ...);

STUPID_CXX_API(void) stupid_string_trim_head(std::string & str, const char * trim = g_blank_character_set);
STUPID_CXX_API(void) stupid_string_trim_tail(std::string & str, const char * trim = g_blank_character_set);
STUPID_CXX_API(void) stupid_string_trim(std::string & str, const char * trim = g_blank_character_set);
STUPID_CXX_API(void) stupid_string_simplify(std::string & str, const char * trim = g_blank_character_set, char simplify = ' ');

STUPID_CXX_API(int) stupid_strcmp_ignore_case(const char * str1, const char * str2);
STUPID_CXX_API(int) stupid_strncmp_ignore_case(const char * str1, const char * str2, int count);

STUPID_CXX_API(bool) stupid_string_to_char  (const char * str, char   & val);
STUPID_CXX_API(bool) stupid_string_to_short (const char * str, short  & val);
STUPID_CXX_API(bool) stupid_string_to_int   (const char * str, int    & val);
STUPID_CXX_API(bool) stupid_string_to_long  (const char * str, long   & val);
STUPID_CXX_API(bool) stupid_string_to_float (const char * str, float  & val);
STUPID_CXX_API(bool) stupid_string_to_double(const char * str, double & val);

STUPID_CXX_API(bool) stupid_string_to_char  (const std::string & str, char   & val);
STUPID_CXX_API(bool) stupid_string_to_short (const std::string & str, short  & val);
STUPID_CXX_API(bool) stupid_string_to_int   (const std::string & str, int    & val);
STUPID_CXX_API(bool) stupid_string_to_long  (const std::string & str, long   & val);
STUPID_CXX_API(bool) stupid_string_to_float (const std::string & str, float  & val);
STUPID_CXX_API(bool) stupid_string_to_double(const std::string & str, double & val);

STUPID_CXX_API(bool) stupid_char_to_string  (char   val, char * str, int size);
STUPID_CXX_API(bool) stupid_short_to_string (short  val, char * str, int size);
STUPID_CXX_API(bool) stupid_int_to_string   (int    val, char * str, int size);
STUPID_CXX_API(bool) stupid_long_to_string  (long   val, char * str, int size);
STUPID_CXX_API(bool) stupid_float_to_string (float  val, char * str, int size);
STUPID_CXX_API(bool) stupid_double_to_string(double val, char * str, int size);

STUPID_CXX_API(bool) stupid_char_to_string  (char   val, std::string & str);
STUPID_CXX_API(bool) stupid_short_to_string (short  val, std::string & str);
STUPID_CXX_API(bool) stupid_int_to_string   (int    val, std::string & str);
STUPID_CXX_API(bool) stupid_long_to_string  (long   val, std::string & str);
STUPID_CXX_API(bool) stupid_float_to_string (float  val, std::string & str);
STUPID_CXX_API(bool) stupid_double_to_string(double val, std::string & str);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_STRING_H
