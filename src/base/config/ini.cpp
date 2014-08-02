/********************************************************
 * Description : ini parse class
 * Data        : 2013-07-27 16:11:54
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <cctype>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "base/config/ini.h"
#include "base/filesystem/sys_io.h"

NAMESPACE_STUPID_BASE_BEGIN

const char * Ini::s_app_head_format[] = { "[", "[ " };
const char * Ini::s_app_tail_format[] = { "]", " ]" };
const char * Ini::s_key_equal_format[] = { "=", " = " };

Ini::KEY_NODE::KEY_NODE(const std::string & key_name, const std::string & key_value)
    : m_key_name(key_name)
    , m_key_value(key_value)
{

}

bool Ini::KEY_NODE::operator == (const std::string & key_name) const
{
    return(key_name == m_key_name);
}

Ini::APP_NODE::APP_NODE(const std::string & app_name)
    : m_app_name(app_name)
    , m_key_list()
{

}

bool Ini::APP_NODE::operator == (const std::string & app_name) const
{
    return(app_name == m_app_name);
}

Ini::Ini()
    : m_file_name()
    , m_comment_char(';')
    , m_support_modify(false)
    , m_format_mode(0)
    , m_need_save(false)
    , m_pair_map()
    , m_app_list()
{

}

Ini::~Ini()
{

}

bool Ini::load(const std::string & file_name, char comment_char, bool support_modify, bool loose)
{
    clear();

    if (isspace(comment_char))
    {
        comment_char = ';';
    }

    m_file_name = file_name;
    m_comment_char = comment_char;
    m_support_modify = support_modify;
    m_format_mode = (loose ? 1 : 0);

    std::ifstream ifs(file_name.c_str());
    if (!ifs.is_open())
    {
        return(support_modify);
    }

    std::string app_name;
    const int bufsiz = 4096;
    char buffer[bufsiz];
    while (!ifs.eof())
    {
        ifs.getline(buffer, bufsiz);

        char * comment_pos = strchr(buffer, comment_char);
        if (nullptr != comment_pos)
        {
            comment_pos[0] = '\0';
        }
        std::string message(buffer);
        stupid_string_trim(message);

        if (message.empty())
        {
            continue;
        }

        if ('[' == message[0])
        {
            std::string::iterator iter_b = message.begin() + 1;
            std::string::iterator iter_e = message.end() - 1;
            if (']' != *iter_e)
            {
                iter_e = message.end();
            }
            std::string(iter_b, iter_e).swap(app_name);
            stupid_string_trim(app_name);

            add_app_node(app_name);
        }
        else
        {
            std::string::iterator iter_k_e;
            std::string::iterator iter_v_s;
            std::string::size_type equal_pos = message.find('=');
            if (std::string::npos == equal_pos)
            {
                iter_k_e = message.end();
                iter_v_s = message.end();
            }
            else
            {
                iter_k_e = message.begin() + equal_pos;
                iter_v_s = message.begin() + equal_pos + 1;
            }

            std::string key_name(message.begin(), iter_k_e);
            std::string key_value(iter_v_s, message.end());
            stupid_string_trim(key_name);
            stupid_string_trim(key_value);

            if (key_name.empty())
            {
                continue;
            }

            if (!m_pair_map.insert(std::make_pair(std::make_pair(app_name, key_name), key_value)).second)
            {
                continue;
            }

            if (support_modify)
            {
                add_app_node(app_name);
                add_key_node(app_name, key_name, key_value);
            }
        }
    }

    ifs.close();

    return(true);
}

bool Ini::save()
{
    if (!m_support_modify)
    {
        return(false);
    }

    if (!m_need_save)
    {
        return(true);
    }

    const std::string tmp_file(m_file_name + "_tmp.ini");

    stupid_rename(m_file_name.c_str(), tmp_file.c_str());

    std::ofstream ofs(m_file_name, std::ios::trunc);
    if (!ofs.is_open())
    {
        return(false);
    }

    APP_ITER app_iter = m_app_list.begin();
    while (m_app_list.end() != app_iter)
    {
        save_app_name(ofs, app_iter->m_app_name);
        KEY_ITER key_iter = app_iter->m_key_list.begin();
        while (app_iter->m_key_list.end() != key_iter)
        {
            save_key_value(ofs, key_iter->m_key_name, key_iter->m_key_value);
            ++key_iter;
        }
        ++app_iter;
    }

    ofs.close();

    stupid_unlink(tmp_file.c_str());

    m_need_save = false;

    return(true);
}

bool Ini::get_value(const std::string & app_name, const std::string & key_name, std::string & key_value) const
{
    if (key_name.empty())
    {
        return(false);
    }

    PAIR_CO_ITER pair_iter = m_pair_map.find(std::make_pair(app_name, key_name));
    if (m_pair_map.end() != pair_iter)
    {
        key_value = pair_iter->second;
        return(true);
    }
    else
    {
        return(false);
    }
}

bool Ini::set_value(const std::string & app_name, const std::string & key_name, const std::string & key_value)
{
    if (!m_support_modify)
    {
        return(false);
    }

    if (key_name.empty())
    {
        return(false);
    }

    APP_ITER app_iter = std::find(m_app_list.begin(), m_app_list.end(), app_name);
    if (m_app_list.end() == app_iter)
    {
        m_app_list.push_back(APP_NODE(app_name));
        app_iter = m_app_list.end();
        --app_iter;
    }
    std::list<KEY_NODE> & key_list = app_iter->m_key_list;
    KEY_ITER key_iter = std::find(key_list.begin(), key_list.end(), key_name);
    if (key_list.end() == key_iter)
    {
        key_list.push_back(KEY_NODE(key_name, key_value));
    }
    else
    {
        key_iter->m_key_value = key_value;
    }

    m_pair_map[std::make_pair(app_name, key_name)] = key_value;

    m_need_save = true;

    return(true);
}

void Ini::add_app_node(const std::string & app_name)
{
    if (m_app_list.end() != std::find(m_app_list.begin(), m_app_list.end(), app_name))
    {
        return;
    }
    m_app_list.push_back(APP_NODE(app_name));
}

void Ini::add_key_node(const std::string & app_name, const std::string & key_name, const std::string & key_value)
{
    APP_RE_ITER app_re_iter = std::find(m_app_list.rbegin(), m_app_list.rend(), app_name);
    if (m_app_list.rend() == app_re_iter)
    {
        return;
    }

    std::list<KEY_NODE> & key_list = app_re_iter->m_key_list;
    if (key_list.end() != std::find(key_list.begin(), key_list.end(), key_name))
    {
        return;
    }

    key_list.push_back(KEY_NODE(key_name, key_value));
}

void Ini::clear()
{
    m_file_name.clear();
    m_comment_char = ';';
    m_support_modify = false;
    m_format_mode = 0;
    m_need_save = false;
    m_pair_map.clear();
    m_app_list.clear();
}

void Ini::save_app_name(std::ofstream & ofs, const std::string & app_name)
{
    if (!app_name.empty())
    {
        ofs << s_app_head_format[m_format_mode] << app_name << s_app_tail_format[m_format_mode] << std::endl;
    }
}

void Ini::save_key_value(std::ofstream & ofs, const std::string & key_name, const std::string & key_value)
{
    ofs << key_name << s_key_equal_format[m_format_mode] << key_value << std::endl;
}

NAMESPACE_STUPID_BASE_END
