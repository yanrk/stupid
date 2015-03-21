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

#include <cassert>
#include "markup.h"
#include "base/config/xml.h"

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
    return(*this);
}

bool Xml::load(const char * file_name)
{
    return(nullptr != file_name && m_markup->Load(file_name));
}

bool Xml::set_document(const char * document)
{
    return(nullptr != document && m_markup->SetDoc(document));
}

bool Xml::find_element(const char * element_name)
{
    if (nullptr == element_name)
    {
        return(false);
    }
    m_markup->ResetMainPos();
    return(m_markup->FindElem(element_name));
}

const std::string & Xml::get_document() const
{
    return(m_markup->GetDoc());
}

bool Xml::get_sub_document(std::string & sub_document)
{
    if (0 == m_markup->FindNode(CMarkup::MNT_ELEMENT))
    {
        return(false);
    }
    sub_document = m_markup->GetSubDoc();
    return(true);
}

bool Xml::get_element(const char * element_name, std::string & element_value)
{
    if (nullptr == element_name)
    {
        return(false);
    }
    m_markup->ResetMainPos();
    if (!m_markup->FindElem(element_name))
    {
        return(false);
    }
    element_value = m_markup->GetData();
    return(true);
}

bool Xml::get_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, std::list<std::string> & child_element_value_list)
{
    if (nullptr == element_name || nullptr == child_element_name)
    {
        return(false);
    }
    m_markup->ResetMainPos();
    if (!m_markup->FindElem(element_name))
    {
        return(false);
    }
    m_markup->ResetChildPos();
    while (m_markup->FindChildElem(child_element_name))
    {
        std::string child_element_value = m_markup->GetChildData();
        if (!ignore_empty_value || !child_element_value.empty())
        {
            child_element_value_list.push_back(child_element_value);
        }
    }
    return(true);
}

bool Xml::get_attribute(const char * attribute_name, std::string & attribute_value)
{
    if (nullptr == attribute_name)
    {
        return(false);
    }
    attribute_value = m_markup->GetAttrib(attribute_name);
    return(true);
}

bool Xml::get_attribute(const char * element_name, const char * attribute_name, std::string & attribute_value)
{
    if (nullptr == element_name || nullptr == attribute_name)
    {
        return(false);
    }
    m_markup->ResetMainPos();
    if (!m_markup->FindElem(element_name))
    {
        return(false);
    }
    attribute_value = m_markup->GetAttrib(attribute_name);
    return(true);
}

bool Xml::get_child_element(const char * child_element_name, std::string & child_element_value)
{
    if (nullptr == child_element_name)
    {
        return(false);
    }
    m_markup->ResetChildPos();
    if (!m_markup->FindChildElem(child_element_name))
    {
        return(false);
    }
    child_element_value = m_markup->GetChildData();
    return(true);
}

bool Xml::get_child_attribute(const char * child_element_name, const char * child_attribute_name, std::string & child_attribute_value)
{
    if (nullptr == child_element_name || nullptr == child_attribute_name)
    {
        return(false);
    }
    m_markup->ResetChildPos();
    if (!m_markup->FindChildElem(child_element_name))
    {
        return(false);
    }
    child_attribute_value = m_markup->GetChildAttrib(child_attribute_name);
    return(true);
}

bool Xml::into_element(const char * element_name)
{
    if (nullptr != element_name)
    {
        m_markup->ResetMainPos();
        if (!m_markup->FindElem(element_name))
        {
            return(false);
        }
    }
    return(m_markup->IntoElem());
}

bool Xml::outof_element()
{
    return(m_markup->OutOfElem());
}

std::string Xml::get_element_content()
{
    return(m_markup->GetElemContent());
}

void Xml::get_element_content(std::string & element_content)
{
    element_content = m_markup->GetElemContent();
}

bool Xml::get_element_content(const char * element_name, std::string & element_content)
{
    if (nullptr == element_name)
    {
        return(false);
    }
    m_markup->ResetMainPos();
    if (!m_markup->FindElem(element_name))
    {
        return(false);
    }
    element_content = m_markup->GetElemContent();
    return(true);
}

bool Xml::save(const char * file_name)
{
    return(nullptr != file_name && m_markup->Save(file_name));
}

bool Xml::add_element(const char * element_name, const char * element_value)
{
    return(nullptr != element_name && m_markup->AddElem(element_name, element_value));
}

bool Xml::add_element(const char * element_name, const std::string & element_value)
{
    return(add_element(element_name, element_value.c_str()));
}

bool Xml::insert_element(const char * element_name, const char * element_value)
{
    return(nullptr != element_name && m_markup->InsertElem(element_name, element_value));
}

bool Xml::insert_element(const char * element_name, const std::string & element_value)
{
    return(insert_element(element_name, element_value.c_str()));
}

bool Xml::add_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<std::string> & child_element_value_list)
{
    if (nullptr == element_name || nullptr == child_element_name)
    {
        return(false);
    }
    if (!m_markup->AddElem(element_name, nullptr))
    {
        return(false);
    }
    for (std::list<std::string>::const_iterator iter = child_element_value_list.begin(); child_element_value_list.end() != iter; ++iter)
    {
        if (!ignore_empty_value || !iter->empty())
        {
            if (!m_markup->AddChildElem(child_element_name, *iter))
            {
                return(false);
            }
        }
    }
    return(true);
}

bool Xml::insert_element_block(const char * element_name, const char * child_element_name, bool ignore_empty_value, const std::list<std::string> & child_element_value_list)
{
    if (nullptr == element_name || nullptr == child_element_name)
    {
        return(false);
    }
    if (!m_markup->InsertElem(element_name, nullptr))
    {
        return(false);
    }
    for (std::list<std::string>::const_iterator iter = child_element_value_list.begin(); child_element_value_list.end() != iter; ++iter)
    {
        if (!ignore_empty_value || !iter->empty())
        {
            if (!m_markup->AddChildElem(child_element_name, *iter))
            {
                return(false);
            }
        }
    }
    return(true);
}

bool Xml::add_child_element(const char * child_element_name, const char * child_element_value)
{
    return(nullptr != child_element_name && m_markup->AddChildElem(child_element_name, child_element_value));
}

bool Xml::add_child_element(const char * child_element_name, const std::string & child_element_value)
{
    return(add_child_element(child_element_name, child_element_value.c_str()));
}

bool Xml::insert_child_element(const char * child_element_name, const char * child_element_value)
{
    return(nullptr != child_element_name && m_markup->InsertChildElem(child_element_name, child_element_value));
}

bool Xml::insert_child_element(const char * child_element_name, const std::string & child_element_value)
{
    return(insert_child_element(child_element_name, child_element_value.c_str()));
}

bool Xml::add_attribute(const char * attribute_name, const char * attribute_value)
{
    if (nullptr == attribute_name || nullptr == attribute_value)
    {
        return(false);
    }
    return(m_markup->AddAttrib(attribute_name, attribute_value));
}

bool Xml::add_attribute(const char * attribute_name, const std::string & attribute_value)
{
    return(add_attribute(attribute_name, attribute_value.c_str()));
}

bool Xml::add_child_attribute(const char * child_attribute_name, const char * child_attribute_value)
{
    if (nullptr == child_attribute_name || nullptr == child_attribute_value)
    {
        return(false);
    }
    return(m_markup->AddChildAttrib(child_attribute_name, child_attribute_value));
}

bool Xml::add_child_attribute(const char * child_attribute_name, const std::string & child_attribute_value)
{
    return(add_child_attribute(child_attribute_name, child_attribute_value.c_str()));
}

bool Xml::add_sub_document(const char * sub_document)
{
    return(nullptr != sub_document && m_markup->AddSubDoc(sub_document));
}

bool Xml::insert_sub_document(const char * sub_document)
{
    return(nullptr != sub_document && m_markup->InsertSubDoc(sub_document));
}

bool Xml::add_child_sub_document(const char * child_sub_document)
{
    return(nullptr != child_sub_document && m_markup->AddChildSubDoc(child_sub_document));
}

bool Xml::insert_child_sub_document(const char * child_sub_document)
{
    return(nullptr != child_sub_document && m_markup->InsertChildSubDoc(child_sub_document));
}

bool Xml::remove_element(const char * element_name)
{
    if (nullptr != element_name)
    {
        m_markup->ResetMainPos();
        if (!m_markup->FindElem(element_name))
        {
            return(true);
        }
    }
    return(m_markup->RemoveElem());
}

bool Xml::remove_child_element(const char * child_element_name)
{
    if (nullptr != child_element_name)
    {
        m_markup->ResetChildPos();
        if (!m_markup->FindChildElem(child_element_name))
        {
            return(true);
        }
    }
    return(m_markup->RemoveChildElem());
}

bool Xml::set_element(const char * element_name, const char * element_value)
{
    if (nullptr == element_name)
    {
        return(false);
    }
    m_markup->ResetMainPos();
    if (m_markup->FindElem(element_name))
    {
        return(m_markup->SetData(element_value));
    }
    else
    {
        return(m_markup->AddElem(element_name, element_value));
    }
}

bool Xml::set_element(const char * element_name, const std::string & element_value)
{
    return(set_element(element_name, element_value.c_str()));
}

bool Xml::set_child_element(const char * child_element_name, const char * child_element_value)
{
    if (nullptr == child_element_name)
    {
        return(false);
    }
    m_markup->ResetChildPos();
    if (m_markup->FindChildElem(child_element_name))
    {
        return(m_markup->SetChildData(child_element_value));
    }
    else
    {
        return(m_markup->AddChildElem(child_element_name, child_element_value));
    }
}

bool Xml::set_child_element(const char * child_element_name, const std::string & child_element_value)
{
    return(set_child_element(child_element_name, child_element_value.c_str()));
}

bool Xml::set_attribute(const char * element_name, const char * attribute_name, const char * attribute_value)
{
    if (nullptr == element_name || nullptr == attribute_name || nullptr == attribute_value)
    {
        return(false);
    }
    m_markup->ResetMainPos();
    if (m_markup->FindElem(element_name))
    {
        return(m_markup->SetAttrib(attribute_name, attribute_value));
    }
    else
    {
        return(m_markup->AddElem(element_name, nullptr) && m_markup->AddAttrib(attribute_name, attribute_value));
    }
}

bool Xml::set_attribute(const char * element_name, const char * attribute_name, const std::string & attribute_value)
{
    return(set_attribute(element_name, attribute_name, attribute_value.c_str()));
}

bool Xml::set_child_attribute(const char * child_element_name, const char * child_attribute_name, const char * child_attribute_value)
{
    if (nullptr == child_element_name || nullptr == child_attribute_name || nullptr == child_attribute_value)
    {
        return(false);
    }
    m_markup->ResetChildPos();
    if (m_markup->FindChildElem(child_element_name))
    {
        return(m_markup->SetChildAttrib(child_attribute_name, child_attribute_value));
    }
    else
    {
        return(m_markup->AddChildElem(child_element_name, nullptr) && m_markup->AddChildAttrib(child_attribute_name, child_attribute_value));
    }
}

bool Xml::set_child_attribute(const char * child_element_name, const char * child_attribute_name, const std::string & child_attribute_value)
{
    return(set_child_attribute(child_element_name, child_attribute_name, child_attribute_value.c_str()));
}

bool Xml::set_element_content(const char * element_name, const char * element_content)
{
    if (nullptr == element_name || nullptr == element_content)
    {
        return(false);
    }
    m_markup->ResetMainPos();
    if (m_markup->FindElem(element_name))
    {
        return(m_markup->SetElemContent(element_content));
    }
    else
    {
        return(m_markup->AddElem(element_name, nullptr) && m_markup->AddSubDoc(element_content));
    }
}

NAMESPACE_STUPID_BASE_END
