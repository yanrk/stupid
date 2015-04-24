/********************************************************
 * Description : ini parse class
 * Data        : 2013-07-27 16:11:54
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_INI_H
#define STUPID_BASE_INI_H


#include <string>
#include <list>
#include <map>
#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/string/string.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API Ini : private Uncopy
{
public:
    Ini();
    ~Ini();

public:
    bool load(const std::string & file_name, char comment_char = ';', bool support_modify = false, bool loose = false);
    bool save();
    void clear();

public:
    bool get_value(const std::string & app_name, const std::string & key_name, std::string & key_value) const;
    bool set_value(const std::string & app_name, const std::string & key_name, const std::string & key_value);
    template <typename T> bool get_value(const std::string & app_name, const std::string & key_name, T & key_value) const;
    template <typename T> bool set_value(const std::string & app_name, const std::string & key_name, T key_value);

private:
    void add_app_node(const std::string & app_name);
    void add_key_node(const std::string & app_name, const std::string & key_name, const std::string & key_value);
    void save_app_name(std::ofstream & ofs, const std::string & app_name);
    void save_key_value(std::ofstream & ofs, const std::string & key_name, const std::string & key_value);

private:
    struct KEY_NODE
    {
        std::string              m_key_name;
        std::string              m_key_value;

        KEY_NODE(const std::string & key_name, const std::string & key_value);
        bool operator == (const std::string & key_name) const;
    };

    struct APP_NODE
    {
        std::string              m_app_name;
        std::list<KEY_NODE>      m_key_list;

        APP_NODE(const std::string & app_name);
        bool operator == (const std::string & app_name) const;
    };

    typedef std::pair<std::string, std::string>                   APP_KEY_PAIR;
    typedef std::map<APP_KEY_PAIR, std::string>::const_iterator   PAIR_CO_ITER;
    typedef std::list<APP_NODE>::iterator                         APP_ITER;
    typedef std::list<APP_NODE>::reverse_iterator                 APP_RE_ITER;
    typedef std::list<KEY_NODE>::iterator                         KEY_ITER;

private:
    static const char *                   s_app_head_format[2];
    static const char *                   s_app_tail_format[2];
    static const char *                   s_key_equal_format[2];

private:
    std::string                           m_file_name;
    char                                  m_comment_char;
    bool                                  m_support_modify;
    size_t                                m_format_mode;
    bool                                  m_need_save;
    std::map<APP_KEY_PAIR, std::string>   m_pair_map;
    std::list<APP_NODE>                   m_app_list;
};

template <typename T>
bool Ini::get_value(const std::string & app_name, const std::string & key_name, T & key_value) const
{
    std::string str_value;
    if (!get_value(app_name, key_name, str_value))
    {
        return(false);
    }
    return(stupid_string_to_type(str_value, key_value));
}

template <typename T>
bool Ini::set_value(const std::string & app_name, const std::string & key_name, T key_value)
{
    std::string str_value;
    if (!stupid_type_to_string(key_value, str_value))
    {
        return(false);
    }
    return(set_value(app_name, key_name, str_value));
}

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_INI_H
