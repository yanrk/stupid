/********************************************************
 * Description : splitter of string
 * Data        : 2014-01-03 18:50:41
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_SPLITTER_H
#define STUPID_BASE_SPLITTER_H


#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API StringSplitter : private Uncopy
{
public:
    StringSplitter(
        const std::string & string_set, 
        const std::string & filter_set = g_blank_character_set, 
        const std::string & splitter_set = ","
    );

public:
    size_t count_bound() const;

public:
    bool has_element() const;
    std::string pop_element();

public:
    bool eof() const;
    StringSplitter & operator >> (std::string & element);

public:
    void rewind();

private:
    std::string                   m_string_set;
    std::string                   m_filter_set;
    std::string                   m_splitter_set;
    std::string::const_iterator   m_iter_b;
    std::string::const_iterator   m_iter_e;
    bool                          m_is_eof;
    size_t                        m_count_bound;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_SPLITTER_H
