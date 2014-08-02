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

#include <cctype>
#include <map>
#include <vector>
#include <string>
#include "tinyxml.h"
#include "base/config/xml.h"
#include "base/string/splitter.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class XmlImpl : private Uncopy
{
public:
    XmlImpl();
    ~XmlImpl();

public:
    bool load(const char * file_name);
    bool parse(const char * xml_content);
    bool get_content(std::string & xml_content);

public:
    bool get_attribute_count(const std::string & route, size_t & attribute_count);
    bool get_attribute_name(const std::string & route, std::string & attribute_name);
    bool get_attribute_value(const std::string & route, std::string & attribute_value);

public:
    bool get_element_count(const std::string & route, size_t & element_count);
    bool get_element_name(const std::string & route, std::string & element_name);
    bool get_element_value(const std::string & route, std::string & element_value);

private:
    TiXmlElement * get_parent_element(const std::string & route, std::string & last_node);

private:
    TiXmlAttribute * get_sub_attribute(TiXmlElement * parent_element, const std::string & attribute_node);
    TiXmlElement * get_sub_element(TiXmlElement * parent_element, const std::string & element_node);

private:
    TiXmlAttribute * get_node_attribute(const std::string & route);
    TiXmlElement * get_node_element(const std::string & route);

private:
    typedef std::map<std::string, TiXmlElement *>                 ELEMENT_MAP;
    typedef std::map<std::string, size_t>                         COUNTER_MAP;

private:
    TiXmlDocument                         m_xml_doc;
    ELEMENT_MAP                           m_element_map;
    COUNTER_MAP                           m_counter_map;
};

XmlImpl::XmlImpl()
    : m_xml_doc()
    , m_element_map()
    , m_counter_map()
{

}

XmlImpl::~XmlImpl()
{
    m_xml_doc.Clear();
    m_element_map.clear();
    m_counter_map.clear();
}

bool XmlImpl::load(const char * file_name)
{
    return(nullptr != file_name && m_xml_doc.LoadFile(file_name));
}

bool XmlImpl::parse(const char * xml_content)
{
    return(nullptr != xml_content && nullptr != m_xml_doc.Parse(xml_content));
}

bool XmlImpl::get_content(std::string & xml_content)
{
    TiXmlPrinter printer;
    if (m_xml_doc.Accept(&printer))
    {
        xml_content = printer.CStr();
        return(true);
    }
    else
    {
        xml_content.clear();
        return(false);
    }
}

TiXmlElement * XmlImpl::get_parent_element(const std::string & route, std::string & last_node)
{
    last_node.clear();

    std::string parent_route;
    std::vector<std::string> route_nodes;

    StringSplitter splitter(route, g_blank_character_set, "/");
    while (splitter.has_element())
    {
        std::string node = splitter.pop_element();
        if (!node.empty())
        {
            parent_route += last_node;
            parent_route += "/";
            last_node = node;
            route_nodes.push_back(node);
        }
    }

    if (route_nodes.empty())
    {
        return(nullptr);
    }

    ELEMENT_MAP::iterator element_iter = m_element_map.find(parent_route);
    if (m_element_map.end() != element_iter)
    {
        return(element_iter->second);
    }

    TiXmlElement * element = m_xml_doc.FirstChildElement(route_nodes.begin()->c_str());

    std::vector<std::string>::iterator iter_b = route_nodes.begin() + 1;
    std::vector<std::string>::iterator iter_e = route_nodes.end() - 1;
    while (iter_b < iter_e)
    {
        const std::string & node = *iter_b;
        ++iter_b;
        element = get_sub_element(element, node);
        if (nullptr == element)
        {
            break;
        }
    }

    m_element_map[parent_route] = element;

    return(element);
}

TiXmlAttribute * XmlImpl::get_sub_attribute(TiXmlElement * parent_element, const std::string & attribute_node)
{
    TiXmlAttribute * sub_attribute = nullptr;

    if (isdigit(attribute_node[0])) // search by attribute-index
    {
        size_t member_index = 0;
        if (!stupid_string_to_type(attribute_node, member_index))
        {
            return(false);
        }

        sub_attribute = parent_element->FirstAttribute();
        while (nullptr != sub_attribute && 0 != member_index)
        {
            --member_index;
            sub_attribute = sub_attribute->Next();
        }
    }
    else // search by attribute-name
    {
        sub_attribute = parent_element->FirstAttribute();
        while (nullptr != sub_attribute && sub_attribute->Name() != attribute_node)
        {
            sub_attribute = sub_attribute->Next();
        }
    }

    return(sub_attribute);
}

TiXmlElement * XmlImpl::get_sub_element(TiXmlElement * parent_element, const std::string & element_node)
{
    TiXmlElement * sub_element = nullptr;

    if (isdigit(element_node[0])) // search by element-index
    {
        size_t member_index = 0;
        if (!stupid_string_to_type(element_node, member_index))
        {
            return(nullptr);
        }

        sub_element = parent_element->FirstChildElement();
        while (nullptr != sub_element && 0 != member_index)
        {
            --member_index;
            sub_element = sub_element->NextSiblingElement();
        }
    }
    else // search by element-name
    {
        sub_element = parent_element->FirstChildElement(element_node.c_str());
    }

    return(sub_element);
}

TiXmlAttribute * XmlImpl::get_node_attribute(const std::string & route)
{
    std::string attribute_node;
    TiXmlElement * parent_element = get_parent_element(route, attribute_node);
    if (nullptr == parent_element)
    {
        return(nullptr);
    }
    return(get_sub_attribute(parent_element, attribute_node));
}

TiXmlElement * XmlImpl::get_node_element(const std::string & route)
{
    std::string element_node;
    TiXmlElement * parent_element = get_parent_element(route, element_node);
    if (nullptr == parent_element)
    {
        return(false);
    }
    return(get_sub_element(parent_element, element_node));
}

bool XmlImpl::get_attribute_count(const std::string & route, size_t & attribute_count)
{
    attribute_count = 0;

    std::string attribute_mark(route);
    attribute_mark += "<"; // append '<' to make a attribute-search-mark
    COUNTER_MAP::iterator counter_iter = m_counter_map.find(attribute_mark);
    if (m_counter_map.end() != counter_iter)
    {
        attribute_count = counter_iter->second;
        return(true);
    }

    std::string dummy;
    TiXmlElement * parent_element = get_parent_element(route + "/*", dummy);
    if (nullptr == parent_element)
    {
        return(false);
    }

    TiXmlAttribute * sub_attribute = parent_element->FirstAttribute();
    while (nullptr != sub_attribute)
    {
        ++attribute_count;
        sub_attribute = sub_attribute->Next();
    }

    m_counter_map[attribute_mark] = attribute_count;

    return(true);
}

bool XmlImpl::get_element_count(const std::string & route, size_t & element_count)
{
    element_count = 0;

    std::string element_mark(route);
    element_mark += ">"; // append '>' to make a element-search-mark
    COUNTER_MAP::iterator counter_iter = m_counter_map.find(element_mark);
    if (m_counter_map.end() != counter_iter)
    {
        element_count = counter_iter->second;
        return(true);
    }

    std::string dummy;
    TiXmlElement * parent_element = get_parent_element(route + "/*", dummy);
    if (nullptr == parent_element)
    {
        return(false);
    }

    TiXmlElement * sub_element = parent_element->FirstChildElement();
    while (nullptr != sub_element)
    {
        ++element_count;
        sub_element = sub_element->NextSiblingElement();
    }

    m_counter_map[element_mark] = element_count;

    return(true);
}

bool XmlImpl::get_attribute_name(const std::string & route, std::string & attribute_name)
{
    attribute_name.clear();

    TiXmlAttribute * attribute = get_node_attribute(route);
    if (nullptr == attribute)
    {
        return(false);
    }

    attribute_name = attribute->Name();

    return(true);
}

bool XmlImpl::get_element_name(const std::string & route, std::string & element_name)
{
    element_name.clear();

    TiXmlElement * element = get_node_element(route);
    if (nullptr == element)
    {
        return(false);
    }

    element_name = element->Value();

    return(true);
}

bool XmlImpl::get_attribute_value(const std::string & route, std::string & attribute_value)
{
    attribute_value.clear();

    TiXmlAttribute * attribute = get_node_attribute(route);
    if (nullptr == attribute)
    {
        return(false);
    }

    attribute_value = attribute->Value();

    return(true);
}

bool XmlImpl::get_element_value(const std::string & route, std::string & element_value)
{
    element_value.clear();

    TiXmlElement * element = get_node_element(route);
    if (nullptr == element)
    {
        return(false);
    }

    element_value = element->GetText();

    return(true);
}

Xml::Xml()
    : m_impl(nullptr)
{

}

Xml::~Xml()
{
    clear();
}

bool Xml::init()
{
    clear();
    STUPID_NEW(m_impl, XmlImpl);
    return(nullptr != m_impl);
}

void Xml::clear()
{
    STUPID_DEL(m_impl);
}

bool Xml::load(const char * file_name)
{
    return(nullptr != file_name && init() && m_impl->load(file_name));
}

bool Xml::load(const std::string & file_name)
{
    return(load(file_name.c_str()));
}

bool Xml::parse(const char * xml_content)
{
    return(nullptr != xml_content && init() && m_impl->parse(xml_content));
}

bool Xml::parse(const std::string & xml_content)
{
    return(parse(xml_content.c_str()));
}

bool Xml::get_content(std::string & xml_content)
{
    return(nullptr != m_impl && m_impl->get_content(xml_content));
}

bool Xml::get_attribute_count(const std::string & route, size_t & attribute_count)
{
    return(nullptr != m_impl && m_impl->get_attribute_count(route, attribute_count));
}

bool Xml::get_attribute_name(const std::string & route, std::string & attribute_name)
{
    return(nullptr != m_impl && m_impl->get_attribute_name(route, attribute_name));
}

bool Xml::get_attribute_value(const std::string & route, std::string & attribute_value)
{
    return(nullptr != m_impl && m_impl->get_attribute_value(route, attribute_value));
}

bool Xml::get_element_count(const std::string & route, size_t & element_count)
{
    return(nullptr != m_impl && m_impl->get_element_count(route, element_count));
}

bool Xml::get_element_name(const std::string & route, std::string & element_name)
{
    return(nullptr != m_impl && m_impl->get_element_name(route, element_name));
}

bool Xml::get_element_value(const std::string & route, std::string & element_value)
{
    return(nullptr != m_impl && m_impl->get_element_value(route, element_value));
}

NAMESPACE_STUPID_BASE_END
