/********************************************************
 * Description : xml parse class
 * Data        : 2014-07-21 14:08:41
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_XML_H
#define STUPID_BASE_XML_H


#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/string/string.h"

NAMESPACE_STUPID_BASE_BEGIN

class XmlImpl;

class STUPID_API Xml : private Uncopy
{
public:
    Xml();
    ~Xml();

public:
    bool load(const char * file_name);
    bool load(const std::string & file_name);
    bool parse(const char * xml_content);
    bool parse(const std::string & xml_content);
    bool get_content(std::string & xml_content);
    void clear();

public:
    bool get_attribute_count(const std::string & route, size_t & attribute_count);
    bool get_attribute_name(const std::string & route, std::string & attribute_name);
    bool get_attribute_value(const std::string & route, std::string & attribute_value);
    template <typename T> bool get_attribute_value(const std::string & route, T & attribute_value);

public:
    bool get_element_count(const std::string & route, size_t & element_count);
    bool get_element_name(const std::string & route, std::string & element_name);
    bool get_element_value(const std::string & route, std::string & element_value);
    template <typename T> bool get_element_value(const std::string & route, T & element_value);

private:
    bool init();

private:
    XmlImpl                             * m_impl;
};

template <typename T>
bool Xml::get_attribute_value(const std::string & route, T & attribute_value)
{
    std::string str_value;
    if (!get_attribute_value(route, str_value))
    {
        return(false);
    }
    return(stupid_string_to_type(str_value, attribute_value));
}

template <typename T>
bool Xml::get_element_value(const std::string & route, T & element_value)
{
    std::string str_value;
    if (!get_element_value(route, str_value))
    {
        return(false);
    }
    return(stupid_string_to_type(str_value, element_value));
}

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_XML_H
