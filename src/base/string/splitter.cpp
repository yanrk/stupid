/********************************************************
 * Description : splitter of string
 * Data        : 2014-01-03 18:50:41
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <algorithm>
#include "base/string/string.h"
#include "base/string/splitter.h"

NAMESPACE_STUPID_BASE_BEGIN

struct is_splitter
{
    is_splitter(const std::string & splitter_set)
        : m_splitter_set(splitter_set)
    {

    }
    bool operator () (const char symb) const
    {
        return m_splitter_set.end() != std::find(m_splitter_set.begin(), m_splitter_set.end(), symb);
    }
    const std::string & m_splitter_set;
};

StringSplitter::StringSplitter(
                    const std::string & string_set, 
                    const std::string & filter_set, 
                    const std::string & splitter_set
                )
    : m_string_set(string_set)
    , m_filter_set(filter_set)
    , m_splitter_set(splitter_set)
    , m_iter_b(m_string_set.begin())
    , m_iter_e(m_string_set.end())
    , m_is_eof(m_iter_b == m_iter_e)
    , m_count_bound(std::count_if(m_iter_b, m_iter_e, is_splitter(m_splitter_set)) + 1)
{

}

size_t StringSplitter::count_bound() const
{
    return m_count_bound;
}

bool StringSplitter::has_element() const
{
    return !eof();
}

bool StringSplitter::eof() const
{
    return m_is_eof;
}

std::string StringSplitter::pop_element()
{
    std::string::const_iterator a_iter_f = 
        std::find_first_of(m_iter_b, m_iter_e, m_splitter_set.begin(), m_splitter_set.end());
    std::string element(m_iter_b, a_iter_f);

    if (a_iter_f != m_iter_e)
    {
        m_iter_b = a_iter_f + 1;
    }
    else
    {
        m_iter_b = m_iter_e;
        m_is_eof = true;
    }

    stupid_string_trim(element, m_filter_set.c_str());

    return element;
}

StringSplitter & StringSplitter::operator >> (std::string & element)
{
    pop_element().swap(element);
    return *this;
}

void StringSplitter::rewind()
{
    m_iter_b = m_string_set.begin();
    m_is_eof = (m_iter_b == m_iter_e);
}

NAMESPACE_STUPID_BASE_END
