/********************************************************
 * Description : xml parse class which base on cmarkup
 * Data        : 2015-03-10 16:14:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cassert>
#include <cstring>
#include "markup.h"
#include "base/config/xml.h"
#include "base/string/string.h"
#include "base/charset/charset.h"

NAMESPACE_STUPID_BASE_BEGIN

Xml::Xml() : m_markup(new CMarkup)
{
    assert(nullptr != m_markup);
}

Xml::~Xml()
{
    delete m_markup;
}

Xml::Xml(const Xml & other) : m_markup(new CMarkup(*other.m_markup))
{
    assert(nullptr != m_markup);
}

Xml & Xml::operator = (const Xml & other)
{
    if (&other != this)
    {
        delete m_markup;
        m_markup = new CMarkup(*other.m_markup);
        assert(nullptr != m_markup);
    }
    return *this;
}

bool Xml::load(const char * file_name)
{
#ifdef _MSC_VER
    return nullptr != file_name && m_markup->Load(utf8_to_unicode(file_name));
#else
    return nullptr != file_name && m_markup->Load(utf8_to_ansi(file_name));
#endif // _MSC_VER
}

bool Xml::set_document(const char * document)
{
    return nullptr != document && m_markup->SetDoc(utf8_to_unicode(document));
}

bool Xml::find_element(const char * element_name)
{
    if (nullptr == element_name)
    {
        return false;
    }
    m_markup->ResetMainPos();
    return m_markup->FindElem(utf8_to_unicode(element_name));
}

std::string Xml::get_document() const
{
    return unicode_to_utf8(m_markup->GetDoc());
}

bool Xml::get_sub_document(std::string & sub_document)
{
    if (0 == m_markup->FindNode(CMarkup::MNT_ELEMENT))
    {
        return false;
    }
    sub_document = unicode_to_utf8(m_markup->GetSubDoc());
    return true;
}

bool Xml::get_element(const char * element_name, std::string & element_value)
{
    if (nullptr == element_name)
    {
        return false;
    }
    m_markup->ResetMainPos();
    if (!m_markup->FindElem(utf8_to_unicode(element_name)))
    {
        return false;
    }
    element_value = unicode_to_utf8(m_markup->GetData());
    stupid_string_trim(element_value);
    return true;
}

bool Xml::get_element(const char * element_name, char * element_value, size_t element_value_size)
{
    std::string str_element_value;
    if (!get_element(element_name, str_element_value))
    {
        return false;
    }
    if (nullptr == element_value || str_element_value.size() >= element_value_size)
    {
        return false;
    }
    strncpy(element_value, str_element_value.c_str(), element_value_size);
    return true;
}

bool Xml::get_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, std::list<std::string> & child_element_value_list)
{
    if (nullptr == element_name || nullptr == child_element_name)
    {
        return false;
    }
    m_markup->ResetMainPos();
    if (!m_markup->FindElem(utf8_to_unicode(element_name)))
    {
        return false;
    }
    m_markup->ResetChildPos();
    while (m_markup->FindChildElem(utf8_to_unicode(child_element_name)))
    {
        std::string child_element_value = unicode_to_utf8(m_markup->GetChildData());
        stupid_string_trim(child_element_value);
        if (!ignore_empty_value || !child_element_value.empty())
        {
            child_element_value_list.push_back(child_element_value);
        }
    }
    return true;
}

bool Xml::get_attribute(const char * attribute_name, std::string & attribute_value)
{
    if (nullptr == attribute_name)
    {
        return false;
    }
    attribute_value = unicode_to_utf8(m_markup->GetAttrib(utf8_to_unicode(attribute_name)));
    stupid_string_trim(attribute_value);
    return true;
}

bool Xml::get_attribute(const char * attribute_name, char * attribute_value, size_t attribute_value_size)
{
    std::string str_attribute_value;
    if (!get_attribute(attribute_name, str_attribute_value))
    {
        return false;
    }
    if (nullptr == attribute_value || str_attribute_value.size() >= attribute_value_size)
    {
        return false;
    }
    strncpy(attribute_value, str_attribute_value.c_str(), attribute_value_size);
    return true;
}

bool Xml::get_attribute(const char * element_name, const char * attribute_name, std::string & attribute_value)
{
    if (nullptr == element_name || nullptr == attribute_name)
    {
        return false;
    }
    m_markup->ResetMainPos();
    if (!m_markup->FindElem(utf8_to_unicode(element_name)))
    {
        return false;
    }
    attribute_value = unicode_to_utf8(m_markup->GetAttrib(utf8_to_unicode(attribute_name)));
    stupid_string_trim(attribute_value);
    return true;
}

bool Xml::get_attribute(const char * element_name, const char * attribute_name, char * attribute_value, size_t attribute_value_size)
{
    std::string str_attribute_value;
    if (!get_attribute(element_name, attribute_name, str_attribute_value))
    {
        return false;
    }
    if (nullptr == attribute_value || str_attribute_value.size() >= attribute_value_size)
    {
        return false;
    }
    strncpy(attribute_value, str_attribute_value.c_str(), attribute_value_size);
    return true;
}

bool Xml::get_child_element(const char * child_element_name, std::string & child_element_value)
{
    if (nullptr == child_element_name)
    {
        return false;
    }
    m_markup->ResetChildPos();
    if (!m_markup->FindChildElem(utf8_to_unicode(child_element_name)))
    {
        return false;
    }
    child_element_value = unicode_to_utf8(m_markup->GetChildData());
    stupid_string_trim(child_element_value);
    return true;
}

bool Xml::get_child_element(const char * child_element_name, char * child_element_value, size_t child_element_value_size)
{
    std::string str_child_element_value;
    if (!get_child_element(child_element_name, str_child_element_value))
    {
        return false;
    }
    if (nullptr == child_element_value || str_child_element_value.size() >= child_element_value_size)
    {
        return false;
    }
    strncpy(child_element_value, str_child_element_value.c_str(), child_element_value_size);
    return true;
}

bool Xml::get_child_attribute(const char * child_element_name, const char * child_attribute_name, std::string & child_attribute_value)
{
    if (nullptr == child_element_name || nullptr == child_attribute_name)
    {
        return false;
    }
    m_markup->ResetChildPos();
    if (!m_markup->FindChildElem(utf8_to_unicode(child_element_name)))
    {
        return false;
    }
    child_attribute_value = unicode_to_utf8(m_markup->GetChildAttrib(utf8_to_unicode(child_attribute_name)));
    stupid_string_trim(child_attribute_value);
    return true;
}

bool Xml::get_child_attribute(const char * child_element_name, const char * child_attribute_name, char * child_attribute_value, size_t child_attribute_value_size)
{
    std::string str_child_attribute_value;
    if (!get_child_attribute(child_element_name, child_attribute_name, str_child_attribute_value))
    {
        return false;
    }
    if (nullptr == child_attribute_value || str_child_attribute_value.size() >= child_attribute_value_size)
    {
        return false;
    }
    strncpy(child_attribute_value, str_child_attribute_value.c_str(), child_attribute_value_size);
    return true;
}

bool Xml::into_element(const char * element_name)
{
    if (nullptr != element_name)
    {
        m_markup->ResetMainPos();
        if (!m_markup->FindElem(utf8_to_unicode(element_name)))
        {
            return false;
        }
    }
    return m_markup->IntoElem();
}

bool Xml::outof_element()
{
    return m_markup->OutOfElem();
}

std::string Xml::get_element_content()
{
    return unicode_to_utf8(m_markup->GetElemContent());
}

void Xml::get_element_content(std::string & element_content)
{
    element_content = unicode_to_utf8(m_markup->GetElemContent());
}

bool Xml::get_element_content(const char * element_name, std::string & element_content)
{
    if (nullptr == element_name)
    {
        return false;
    }
    m_markup->ResetMainPos();
    if (!m_markup->FindElem(utf8_to_unicode(element_name)))
    {
        return false;
    }
    element_content = unicode_to_utf8(m_markup->GetElemContent());
    return true;
}

bool Xml::save(const char * file_name)
{
#ifdef _MSC_VER
    return nullptr != file_name && m_markup->Save(utf8_to_unicode(file_name));
#else
    return nullptr != file_name && m_markup->Save(utf8_to_ansi(file_name));
#endif // _MSC_VER
}

bool Xml::add_element(const char * element_name, const char * element_value)
{
    if (nullptr == element_value)
    {
        return nullptr != element_name && m_markup->AddElem(utf8_to_unicode(element_name), nullptr);
    }
    else
    {
        return nullptr != element_name && m_markup->AddElem(utf8_to_unicode(element_name), utf8_to_unicode(element_value));
    }
}

bool Xml::add_element(const char * element_name, const std::string & element_value)
{
    return add_element(element_name, element_value.c_str());
}

bool Xml::insert_element(const char * element_name, const char * element_value)
{
    if (nullptr == element_value)
    {
        return nullptr != element_name && m_markup->InsertElem(utf8_to_unicode(element_name), nullptr);
    }
    else
    {
        return nullptr != element_name && m_markup->InsertElem(utf8_to_unicode(element_name), utf8_to_unicode(element_value));
    }
}

bool Xml::insert_element(const char * element_name, const std::string & element_value)
{
    return insert_element(element_name, element_value.c_str());
}

bool Xml::add_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<std::string> & child_element_value_list)
{
    if (nullptr == element_name || nullptr == child_element_name)
    {
        return false;
    }
    if (!m_markup->AddElem(utf8_to_unicode(element_name), nullptr))
    {
        return false;
    }
    for (std::list<std::string>::const_iterator iter = child_element_value_list.begin(); child_element_value_list.end() != iter; ++iter)
    {
        if (!ignore_empty_value || !iter->empty())
        {
            if (!m_markup->AddChildElem(utf8_to_unicode(child_element_name), utf8_to_unicode(*iter)))
            {
                return false;
            }
        }
    }
    return true;
}

bool Xml::insert_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<std::string> & child_element_value_list)
{
    if (nullptr == element_name || nullptr == child_element_name)
    {
        return false;
    }
    if (!m_markup->InsertElem(utf8_to_unicode(element_name), nullptr))
    {
        return false;
    }
    for (std::list<std::string>::const_iterator iter = child_element_value_list.begin(); child_element_value_list.end() != iter; ++iter)
    {
        if (!ignore_empty_value || !iter->empty())
        {
            if (!m_markup->AddChildElem(utf8_to_unicode(child_element_name), utf8_to_unicode(*iter)))
            {
                return false;
            }
        }
    }
    return true;
}

bool Xml::add_child_element(const char * child_element_name, const char * child_element_value)
{
    if (nullptr == child_element_value)
    {
        return nullptr != child_element_name && m_markup->AddChildElem(utf8_to_unicode(child_element_name), nullptr);
    }
    else
    {
        return nullptr != child_element_name && m_markup->AddChildElem(utf8_to_unicode(child_element_name), utf8_to_unicode(child_element_value));
    }
}

bool Xml::add_child_element(const char * child_element_name, const std::string & child_element_value)
{
    return add_child_element(child_element_name, child_element_value.c_str());
}

bool Xml::insert_child_element(const char * child_element_name, const char * child_element_value)
{
    if (nullptr == child_element_value)
    {
        return nullptr != child_element_name && m_markup->InsertChildElem(utf8_to_unicode(child_element_name), nullptr);
    }
    else
    {
        return nullptr != child_element_name && m_markup->InsertChildElem(utf8_to_unicode(child_element_name), utf8_to_unicode(child_element_value));
    }
}

bool Xml::insert_child_element(const char * child_element_name, const std::string & child_element_value)
{
    return insert_child_element(child_element_name, child_element_value.c_str());
}

bool Xml::add_attribute(const char * attribute_name, const char * attribute_value)
{
    if (nullptr == attribute_name || nullptr == attribute_value)
    {
        return false;
    }
    return m_markup->AddAttrib(utf8_to_unicode(attribute_name), utf8_to_unicode(attribute_value));
}

bool Xml::add_attribute(const char * attribute_name, const std::string & attribute_value)
{
    return add_attribute(attribute_name, attribute_value.c_str());
}

bool Xml::add_child_attribute(const char * child_attribute_name, const char * child_attribute_value)
{
    if (nullptr == child_attribute_name || nullptr == child_attribute_value)
    {
        return false;
    }
    return m_markup->AddChildAttrib(utf8_to_unicode(child_attribute_name), utf8_to_unicode(child_attribute_value));
}

bool Xml::add_child_attribute(const char * child_attribute_name, const std::string & child_attribute_value)
{
    return add_child_attribute(child_attribute_name, child_attribute_value.c_str());
}

bool Xml::add_sub_document(const char * sub_document)
{
    return nullptr != sub_document && m_markup->AddSubDoc(utf8_to_unicode(sub_document));
}

bool Xml::insert_sub_document(const char * sub_document)
{
    return nullptr != sub_document && m_markup->InsertSubDoc(utf8_to_unicode(sub_document));
}

bool Xml::add_child_sub_document(const char * child_sub_document)
{
    return nullptr != child_sub_document && m_markup->AddChildSubDoc(utf8_to_unicode(child_sub_document));
}

bool Xml::insert_child_sub_document(const char * child_sub_document)
{
    return nullptr != child_sub_document && m_markup->InsertChildSubDoc(utf8_to_unicode(child_sub_document));
}

bool Xml::remove_element(const char * element_name)
{
    if (nullptr != element_name)
    {
        m_markup->ResetMainPos();
        if (!m_markup->FindElem(utf8_to_unicode(element_name)))
        {
            return true;
        }
    }
    return m_markup->RemoveElem();
}

bool Xml::remove_child_element(const char * child_element_name)
{
    if (nullptr != child_element_name)
    {
        m_markup->ResetChildPos();
        if (!m_markup->FindChildElem(utf8_to_unicode(child_element_name)))
        {
            return true;
        }
    }
    return m_markup->RemoveChildElem();
}

bool Xml::set_element(const char * element_name, const char * element_value)
{
    if (nullptr == element_name)
    {
        return false;
    }
    m_markup->ResetMainPos();
    if (nullptr == element_value)
    {
        if (m_markup->FindElem(utf8_to_unicode(element_name)))
        {
            return m_markup->SetData(nullptr);
        }
        else
        {
            return m_markup->AddElem(utf8_to_unicode(element_name), nullptr);
        }
    }
    else
    {
        if (m_markup->FindElem(utf8_to_unicode(element_name)))
        {
            return m_markup->SetData(utf8_to_unicode(element_value));
        }
        else
        {
            return m_markup->AddElem(utf8_to_unicode(element_name), utf8_to_unicode(element_value));
        }
    }
}

bool Xml::set_element(const char * element_name, const std::string & element_value)
{
    return set_element(element_name, element_value.c_str());
}

bool Xml::set_child_element(const char * child_element_name, const char * child_element_value)
{
    if (nullptr == child_element_name)
    {
        return false;
    }
    m_markup->ResetChildPos();
    if (nullptr == child_element_value)
    {
        if (m_markup->FindChildElem(utf8_to_unicode(child_element_name)))
        {
            return m_markup->SetChildData(nullptr);
        }
        else
        {
            return m_markup->AddChildElem(utf8_to_unicode(child_element_name), nullptr);
        }
    }
    else
    {
        if (m_markup->FindChildElem(utf8_to_unicode(child_element_name)))
        {
            return m_markup->SetChildData(utf8_to_unicode(child_element_value));
        }
        else
        {
            return m_markup->AddChildElem(utf8_to_unicode(child_element_name), utf8_to_unicode(child_element_value));
        }
    }
}

bool Xml::set_child_element(const char * child_element_name, const std::string & child_element_value)
{
    return set_child_element(child_element_name, child_element_value.c_str());
}

bool Xml::set_attribute(const char * element_name, const char * attribute_name, const char * attribute_value)
{
    if (nullptr == element_name || nullptr == attribute_name || nullptr == attribute_value)
    {
        return false;
    }
    m_markup->ResetMainPos();
    if (m_markup->FindElem(utf8_to_unicode(element_name)))
    {
        return m_markup->SetAttrib(utf8_to_unicode(attribute_name), utf8_to_unicode(attribute_value));
    }
    else
    {
        return m_markup->AddElem(utf8_to_unicode(element_name), nullptr) && m_markup->AddAttrib(utf8_to_unicode(attribute_name), utf8_to_unicode(attribute_value));
    }
}

bool Xml::set_attribute(const char * element_name, const char * attribute_name, const std::string & attribute_value)
{
    return set_attribute(element_name, attribute_name, attribute_value.c_str());
}

bool Xml::set_child_attribute(const char * child_element_name, const char * child_attribute_name, const char * child_attribute_value)
{
    if (nullptr == child_element_name || nullptr == child_attribute_name || nullptr == child_attribute_value)
    {
        return false;
    }
    m_markup->ResetChildPos();
    if (m_markup->FindChildElem(utf8_to_unicode(child_element_name)))
    {
        return m_markup->SetChildAttrib(utf8_to_unicode(child_attribute_name), utf8_to_unicode(child_attribute_value));
    }
    else
    {
        return m_markup->AddChildElem(utf8_to_unicode(child_element_name), nullptr) && m_markup->AddChildAttrib(utf8_to_unicode(child_attribute_name), utf8_to_unicode(child_attribute_value));
    }
}

bool Xml::set_child_attribute(const char * child_element_name, const char * child_attribute_name, const std::string & child_attribute_value)
{
    return set_child_attribute(child_element_name, child_attribute_name, child_attribute_value.c_str());
}

bool Xml::set_element_content(const char * element_name, const char * element_content)
{
    if (nullptr == element_name || nullptr == element_content)
    {
        return false;
    }
    m_markup->ResetMainPos();
    if (m_markup->FindElem(utf8_to_unicode(element_name)))
    {
        return m_markup->SetElemContent(utf8_to_unicode(element_content));
    }
    else
    {
        return m_markup->AddElem(utf8_to_unicode(element_name), nullptr) && m_markup->AddSubDoc(utf8_to_unicode(element_content));
    }
}

NAMESPACE_STUPID_BASE_END
