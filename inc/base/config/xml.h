/********************************************************
 * Description : xml parse class which base on cmarkup
 * Data        : 2015-03-10 16:14:00
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_XML_H
#define STUPID_BASE_XML_H


#include <string>
#include <list>
#include "base/common/common.h"
#include "base/string/string.h"

class CMarkup;

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API Xml
{
public:
    Xml();
    ~Xml();

public:
    Xml(const Xml & other);
    Xml & operator = (const Xml & other);

public: /* read */
    bool load(const char * file_name);
    bool set_document(const char * document);
    bool find_element(const char * element_name);
    const std::string & get_document() const;
    bool get_sub_document(std::string & sub_document);
    bool get_element(const char * element_name, std::string & element_value);
    bool get_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, std::list<std::string> & child_element_value_list);
    bool get_attribute(const char * attribute_name, std::string & attribute_value);
    bool get_attribute(const char * element_name, const char * attribute_name, std::string & attribute_value);
    bool get_child_element(const char * child_element_name, std::string & child_element_value);
    bool get_child_attribute(const char * child_element_name, const char * child_attribute_name, std::string & child_attribute_value);
    bool into_element(const char * element_name = nullptr);
    bool outof_element();
    std::string get_element_content();
    void get_element_content(std::string & element_content);
    bool get_element_content(const char * element_name, std::string & element_content);

public: /* write */
    bool save(const char * file_name);
    bool add_element(const char * element_name, const char * element_value = nullptr);
    bool add_element(const char * element_name, const std::string & element_value);
    bool insert_element(const char * element_name, const char * element_value = nullptr);
    bool insert_element(const char * element_name, const std::string & element_value);
    bool add_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<std::string> & child_element_value_list);
    bool insert_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<std::string> & child_element_value_list);
    bool add_child_element(const char * child_element_name, const char * child_element_value = nullptr);
    bool add_child_element(const char * child_element_name, const std::string & child_element_value);
    bool insert_child_element(const char * child_element_name, const char * child_element_value = nullptr);
    bool insert_child_element(const char * child_element_name, const std::string & child_element_value);
    bool add_attribute(const char * attribute_name, const char * attribute_value);
    bool add_child_attribute(const char * child_attribute_name, const char * child_attribute_value);
    bool add_sub_document(const char * sub_document);
    bool insert_sub_document(const char * sub_document);
    bool add_child_sub_document(const char * child_sub_document);
    bool insert_child_sub_document(const char * child_sub_document);

public: /* modify */
    bool remove_element(const char * element_name = nullptr);
    bool remove_child_element(const char * child_element_name = nullptr);
    bool set_element(const char * element_name, const char * element_value = nullptr);
    bool set_element(const char * element_name, const std::string & element_value);
    bool set_child_element(const char * child_element_name, const char * child_element_value = nullptr);
    bool set_child_element(const char * child_element_name, const std::string & child_element_value);
    bool set_attribute(const char * element_name, const char * attribute_name, const char * attribute_value);
    bool set_child_attribute(const char * child_element_name, const char * child_attribute_name, const char * child_attribute_value);
    bool set_element_content(const char * element_name, const char * element_content);

public: /* template */
    template <typename T> bool get_element(const char * element_name, T & element_value);
    template <typename T> bool get_child_element(const char * child_element_name, T & child_element_value);
    template <typename T> bool add_element(const char * element_name, T element_value);
    template <typename T> bool insert_element(const char * element_name, T element_value);
    template <typename T> bool add_child_element(const char * child_element_name, T child_element_value);
    template <typename T> bool insert_child_element(const char * child_element_name, T child_element_value);
    template <typename T> bool set_element(const char * element_name, T element_value);
    template <typename T> bool set_child_element(const char * child_element_name, T child_element_value);
    template <typename T> bool get_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, std::list<T> & child_element_value_list);
    template <typename T> bool add_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<T> & child_element_value_list);
    template <typename T> bool insert_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<T> & child_element_value_list);

private:
    CMarkup         * m_markup;
};

template <typename T>
bool Xml::get_element(const char * element_name, T & element_value)
{
    std::string str_element_value;
    if (!get_element(element_name, str_element_value))
    {
        return(false);
    }
    return(stupid_string_to_type(str_element_value, element_value));
}

template <typename T>
bool Xml::get_child_element(const char * child_element_name, T & child_element_value)
{
    std::string str_child_element_value;
    if (!get_child_element(child_element_name, str_child_element_value))
    {
        return(false);
    }
    return(stupid_string_to_type(str_child_element_value, child_element_value));
}

template <typename T>
bool Xml::add_element(const char * element_name, T element_value)
{
    std::string str_element_value;
    if (!stupid_type_to_string(element_value, str_element_value))
    {
        return(false);
    }
    return(add_element(element_name, str_element_value));
}

template <typename T>
bool Xml::insert_element(const char * element_name, T element_value)
{
    std::string str_element_value;
    if (!stupid_type_to_string(element_value, str_element_value))
    {
        return(false);
    }
    return(insert_element(element_name, str_element_value));
}

template <typename T>
bool Xml::add_child_element(const char * child_element_name, T child_element_value)
{
    std::string str_child_element_value;
    if (!stupid_type_to_string(child_element_value, str_child_element_value))
    {
        return(false);
    }
    return(add_child_element(child_element_name, str_child_element_value));
}

template <typename T>
bool Xml::insert_child_element(const char * child_element_name, T child_element_value)
{
    std::string str_child_element_value;
    if (!stupid_type_to_string(child_element_value, str_child_element_value))
    {
        return(false);
    }
    return(insert_child_element(child_element_name, str_child_element_value));
}

template <typename T>
bool Xml::set_element(const char * element_name, T element_value)
{
    std::string str_element_value;
    if (!stupid_type_to_string(element_value, str_element_value))
    {
        return(false);
    }
    return(set_element(element_name, str_element_value));
}

template <typename T>
bool Xml::set_child_element(const char * child_element_name, T child_element_value)
{
    std::string str_child_element_value;
    if (!stupid_type_to_string(child_element_value, str_child_element_value))
    {
        return(false);
    }
    return(set_child_element(child_element_name, str_child_element_value));
}

template <typename T>
bool Xml::get_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, std::list<T> & child_element_value_list)
{
    std::list<std::string> str_child_element_value_list;
    if (!get_element_block(element_name, child_element_name, ignore_empty_value, str_child_element_value_list))
    {
        return(false);
    }
    return(stupid_string_to_type(str_child_element_value_list, child_element_value_list));
}

template <typename T>
bool Xml::add_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<T> & child_element_value_list)
{
    std::list<std::string> str_child_element_value_list;
    if (!stupid_type_to_string(child_element_value_list, str_child_element_value_list))
    {
        return(false);
    }
    return(add_element_block(element_name, child_element_name, ignore_empty_value, str_child_element_value_list));
}

template <typename T>
bool Xml::insert_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<T> & child_element_value_list)
{
    std::list<std::string> str_child_element_value_list;
    if (!stupid_type_to_string(child_element_value_list, str_child_element_value_list))
    {
        return(false);
    }
    return(insert_element_block(element_name, child_element_name, ignore_empty_value, str_child_element_value_list));
}

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_XML_H
