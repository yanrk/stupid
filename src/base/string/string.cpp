/********************************************************
 * Description : string operator functions
 * Data        : 2013-05-18 20:41:33
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cstring>
#include "base/string/string.h"

NAMESPACE_STUPID_BASE_BEGIN

size_t stupid_vsnprintf(char * buffer, size_t bufsiz, const char * fmt, va_list args)
{
    if (nullptr == buffer || 0 == bufsiz || nullptr == fmt || nullptr == args)
    {
        return(0);
    }

    size_t ret_siz = vsnprintf(buffer, bufsiz, fmt, args);

    if (bufsiz <= ret_siz)
    {
        ret_siz = bufsiz - 1;
        buffer[ret_siz] = '\0';
    }

    return(ret_siz);
}

size_t stupid_snprintf(char * buffer, size_t bufsiz, const char * fmt, ...)
{
    if (nullptr == fmt)
    {
        return(0);
    }

    va_list args;

    va_start(args, fmt);

    size_t ret_siz = stupid_vsnprintf(buffer, bufsiz, fmt, args);

    va_end(args);

    return(ret_siz);
}

void stupid_string_trim_head(std::string & str, const char * trim)
{
    if (nullptr == trim || '\0' == trim[0])
    {
        return;
    }

    std::string::size_type h_pos = str.find_first_not_of(trim);
    if (std::string::npos == h_pos)
    {
        str.clear();
    }
    else if (0 != h_pos)
    {
        std::string(str.begin() + h_pos, str.end()).swap(str);
    }
}

void stupid_string_trim_tail(std::string & str, const char * trim)
{
    if (nullptr == trim || '\0' == trim[0])
    {
        return;
    }

    std::string::size_type t_pos = str.find_last_not_of(trim);
    if (std::string::npos == t_pos)
    {
        str.clear();
    }
    else if (str.size() - 1 != t_pos)
    {
        std::string(str.begin(), str.begin() + t_pos + 1).swap(str);
    }
}

void stupid_string_trim(std::string & str, const char * trim)
{
    if (nullptr == trim || '\0' == trim[0])
    {
        return;
    }

    std::string::size_type h_pos = str.find_first_not_of(trim);
    if (std::string::npos == h_pos)
    {
        str.clear();
    }
    else
    {
        std::string::size_type t_pos = str.find_last_not_of(trim);
        if (0 != h_pos || str.size() - 1 != t_pos)
        {
            std::string(str.begin() + h_pos, str.begin() + t_pos + 1).swap(str);
        }
    }
}

void stupid_string_simplify(std::string & str, const char * trim, char simplify)
{
    if (nullptr == trim || '\0' == trim[0])
    {
        return;
    }

    std::string trim_set(trim);

    std::string::size_type pos = str.find_first_not_of(trim_set);
    if (std::string::npos == pos)
    {
        str.clear();
        return;
    }

    std::string::iterator iter_b = str.begin() + pos;
    std::string::iterator iter_e = str.begin() + str.find_last_not_of(trim_set) + 1;
    bool need_replace_blank_to_simplify_character = true;
    std::string::iterator iter_r = iter_b;
    std::string::iterator iter_w = iter_b;
    while (iter_r != iter_e)
    {
        if (std::string::npos == trim_set.find(*iter_r))
        {
            if (iter_w != iter_r)
            {
                *iter_w = *iter_r;
            }
            ++iter_w;
            need_replace_blank_to_simplify_character = true;
        }
        else if (need_replace_blank_to_simplify_character)
        {
            *iter_w = simplify;
            ++iter_w;
            need_replace_blank_to_simplify_character = false;
        }
        ++iter_r;
    }
    std::string(iter_b, iter_w).swap(str);
}

int stupid_strcmp_ignore_case(const char * str1, const char * str2)
{
#ifdef _MSC_VER
    return(stricmp(str1, str2));
#else
    return(strcasecmp(str1, str2));
#endif // _MSC_VER
}

int stupid_strncmp_ignore_case(const char * str1, const char * str2, int count)
{
#ifdef _MSC_VER
    return(strnicmp(str1, str2, count));
#else
    return(strncasecmp(str1, str2, count));
#endif // _MSC_VER
}

#define CSTRING_TO_TYPE(type, fmt)                                  \
bool stupid_string_to_##type(const char * str, type & val)          \
{                                                                   \
    if (nullptr == str)                                             \
    {                                                               \
        return(false);                                              \
    }                                                               \
    return(sscanf(str, fmt, &val) > 0);                             \
}

#define STRING_TO_TYPE(type)                                        \
bool stupid_string_to_##type(const std::string & str, type & val)   \
{                                                                   \
    return(stupid_string_to_##type(str.c_str(), val));              \
}

#define TYPE_TO_CSTRING(type, fmt)                                  \
bool stupid_##type##_to_string(type val, char * str, int size)      \
{                                                                   \
    return(stupid_snprintf(str, size, fmt, val) > 0);               \
}

#define TYPE_TO_STRING(type)                                        \
bool stupid_##type##_to_string(type val, std::string & str)         \
{                                                                   \
    const int bufsiz = 64;                                          \
    char buffer[bufsiz];                                            \
    stupid_##type##_to_string(val, buffer, bufsiz);                 \
    str = buffer;                                                   \
    return(true);                                                   \
}

CSTRING_TO_TYPE(  char,  "%c")
CSTRING_TO_TYPE( short, "%hd")
CSTRING_TO_TYPE(   int,  "%d")
CSTRING_TO_TYPE(  long, "%ld")
CSTRING_TO_TYPE( float,  "%f")
CSTRING_TO_TYPE(double, "%lf")

STRING_TO_TYPE(   char)
STRING_TO_TYPE(  short)
STRING_TO_TYPE(    int)
STRING_TO_TYPE(   long)
STRING_TO_TYPE(  float)
STRING_TO_TYPE( double)

TYPE_TO_CSTRING(  char,  "%c")
TYPE_TO_CSTRING( short,  "%d")
TYPE_TO_CSTRING(   int,  "%d")
TYPE_TO_CSTRING(  long, "%ld")
TYPE_TO_CSTRING( float,  "%f")
TYPE_TO_CSTRING(double,  "%f")

TYPE_TO_STRING(   char)
TYPE_TO_STRING(  short)
TYPE_TO_STRING(    int)
TYPE_TO_STRING(   long)
TYPE_TO_STRING(  float)
TYPE_TO_STRING( double)

NAMESPACE_STUPID_BASE_END
