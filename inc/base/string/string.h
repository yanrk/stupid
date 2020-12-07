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

#ifndef STUPID_BASE_STRING_H
#define STUPID_BASE_STRING_H


#include <cstdarg>
#include <cstring>
#include <string>
#include <list>
#include <sstream>
#include <algorithm>
#include "base/common/common.h"
#include "base/string/splitter.h"

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
bool stupid_string_to_type(const std::string & str, T * val);

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

template <typename T>
bool stupid_type_to_string(T * val, std::string & str);

template <typename T>
bool stupid_string_to_type(const std::list<std::string> & str_list, std::list<T> & val_list)
{
    bool ret = true;
    for (std::list<std::string>::const_iterator iter = str_list.begin(); str_list.end() != iter; ++iter)
    {
        T val;
        if (stupid_string_to_type(*iter, val))
        {
            val_list.push_back(val);
        }
        else
        {
            ret = false;
        }
    }
    return(ret);
}

template <typename T>
bool stupid_type_to_string(const std::list<T> & val_list, std::list<std::string> & str_list)
{
    bool ret = true;
    for (typename std::list<T>::const_iterator iter = val_list.begin(); val_list.end() != iter; ++iter)
    {
        std::string str;
        if (stupid_type_to_string(*iter, str))
        {
            str_list.push_back(str);
        }
        else
        {
            ret = false;
        }
    }
    return(ret);
}

template <typename IteratorAll, typename IteratorPart>
bool stupid_includes(IteratorAll first1, IteratorAll last1, IteratorPart first2, IteratorPart last2)
{
    while (first2 != last2)
    {
        if (last1 == std::find(first1, last1, *first2))
        {
            return(false);
        }
        ++first2;
    }
    return(true);
}

template <typename StringIterator>
void stupid_piece_together(StringIterator first, StringIterator last, const std::string & delimiter, std::string & result)
{
    for (StringIterator iter = first; iter != last; ++iter)
    {
        if (iter != first)
        {
            result += delimiter;
        }
        result += *iter;
    }
}

template <typename StringSequence>
void stupid_split_piece(const std::string & values, const std::string & delimiter, bool trim_space, bool ignore_empty, StringSequence & result)
{
    std::string filter(trim_space ? g_blank_character_set : "");
    StringSplitter splitter(values, filter, delimiter);
    while (!splitter.eof())
    {
        std::string element;
        splitter >> element;
        if (!ignore_empty || !element.empty())
        {
            result.push_back(element);
        }
    }
}

STUPID_CXX_API(void) stupid_string_trim_head(std::string & str, const char * trim = g_blank_character_set);
STUPID_CXX_API(void) stupid_string_trim_tail(std::string & str, const char * trim = g_blank_character_set);
STUPID_CXX_API(void) stupid_string_trim(std::string & str, const char * trim = g_blank_character_set);
STUPID_CXX_API(void) stupid_string_simplify(std::string & str, const char * trim = g_blank_character_set, char simplify = ' ');

template <typename StringSequence>
bool stupid_split_command_line(const char * command_line, StringSequence & result, const char * delimiter_set = " \"", bool trim_delimiter = true)
{
    if (nullptr == command_line)
    {
        return(false);
    }

    if (nullptr == delimiter_set || '\0' == delimiter_set[0])
    {
        delimiter_set = " ";
    }

    const char * first = command_line;
    const char * last = nullptr;

    while ('\0' != *first)
    {
        while (' ' == *first || '\t' == *first)
        {
            ++first;
        }

        if ('\0' == *first)
        {
            break;
        }

        char delimiter = ' ';
        if (nullptr != strchr(delimiter_set, *first))
        {
            delimiter = *first;
        }

        last = first + 1;

        while ('\0' != *last)
        {
            if (' ' == delimiter)
            {
                if (' ' == *last || '\t' == *last)
                {
                    break;
                }
            }
            else
            {
                if (delimiter == *last)
                {
                    break;
                }
            }
            ++last;
        }

        if ('\0' == *last && ' ' != delimiter)
        {
            return(false);
        }

        if ('\0' != *last)
        {
            ++last;
        }

        std::string param(first, last);
        stupid_string_trim(param);
        if (trim_delimiter)
        {
            const char trim_set[2] = { delimiter, 0x00 };
            stupid_string_trim(param, trim_set);
        }
        result.push_back(param);

        first = last;
    }

    return(true);
}

STUPID_CXX_API(size_t) stupid_vsnprintf(char * buffer, size_t bufsiz, const char * fmt, va_list args);
STUPID_CXX_API(size_t) stupid_snprintf(char * buffer, size_t bufsiz, const char * fmt, ...);

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
