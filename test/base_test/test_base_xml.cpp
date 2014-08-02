#include <fstream>
#include <sstream>
#include "test_base.h"
#include "base/config/xml.h"
#include "base/string/string.h"

USING_NAMESPACE_STUPID_BASE

static bool print_attribute(Xml & xml, const std::string & attribute_node)
{
    std::string attribute_name;
    if (!xml.get_attribute_name(attribute_node, attribute_name))
    {
        printf("get_attribute_name(%s) failed\n", attribute_node.c_str());
        return(false);
    }
    std::string attribute_value;
    if (!xml.get_attribute_value(attribute_node, attribute_value))
    {
        printf("get_attribute_value(%s) failed\n", attribute_node.c_str());
        return(false);
    }

    printf("A(%s) -> (%s, %s)\n", attribute_node.c_str(), attribute_name.c_str(), attribute_value.c_str());

    return(true);
}

static bool print_element(Xml & xml, const std::string & element_node)
{
    std::string element_name;
    if (!xml.get_element_name(element_node, element_name))
    {
        printf("get_element_name(%s) failed\n", element_node.c_str());
        return(false);
    }
    std::string element_value;
    if (!xml.get_element_value(element_node, element_value))
    {
        printf("get_element_value(%s) failed\n", element_node.c_str());
        return(false);
    }

    printf("E(%s) -> (%s, %s)\n", element_node.c_str(), element_name.c_str(), element_value.c_str());

    return(true);
}

static bool print_xml_node_info(Xml & xml, const std::string & route)
{
    size_t attribute_count = 0;
    if (!xml.get_attribute_count(route, attribute_count))
    {
        printf("get_attribute_count(%s) failed\n", route.c_str());
        return(false);
    }
    for (size_t index = 0; index < attribute_count; ++index)
    {
        std::string str_index;
        stupid_type_to_string(index, str_index);
        std::string str_node(route + "/" + str_index);
        if (!print_attribute(xml, str_node))
        {
            return(false);
        }
    }

    size_t element_count = 0;
    if (!xml.get_element_count(route, element_count))
    {
        printf("get_element_count(%s) failed\n", route.c_str());
        return(false);
    }
    for (size_t index = 0; index < element_count; ++index)
    {
        std::string str_index;
        stupid_type_to_string(index, str_index);
        std::string str_node(route + "/" + str_index);
        if (!print_element(xml, str_node))
        {
            return(false);
        }
    }

    return(true);
}

static void test_load_xml(Xml & xml)
{
    const char * file_name = "./test.xml";

    if (!xml.load(file_name))
    {
        printf("load %s failed\n", file_name);
        return;
    }

    printf("--------------------------------\n\n");

    if (!print_xml_node_info(xml, "test_root/test1"))
    {
        return;
    }

    printf("--------------------------------\n");

    if (!print_attribute(xml, "test_root/test2/attribute_3_name"))
    {
        return;
    }

    printf("--------------------------------\n");

    if (!print_element(xml, "test_root/test3/element_3_name"))
    {
        return;
    }

    printf("--------------------------------\n");

    std::string xml_content;
    if (!xml.get_content(xml_content))
    {
        printf("get_content failed\n");
        return;
    }
    printf("content:\n%s\n", xml_content.c_str());

    printf("--------------------------------\n");
}

static void test_parse_xml(Xml & xml)
{
    const char * file_name = "./test.xml";
    std::ifstream ifs(file_name);
    if (!ifs.is_open())
    {
        printf("open %s failed\n", file_name);
        return;
    }

    std::ostringstream oss;
    oss << ifs.rdbuf();
    ifs.close();

    if (!xml.parse(oss.str().c_str()))
    {
        printf("parse xml-content failed\n");
        return;
    }

    printf("--------------------------------\n\n");

    if (!print_xml_node_info(xml, "test_root/test1"))
    {
        return;
    }

    printf("--------------------------------\n");

    if (!print_attribute(xml, "test_root/test2/attribute_3_name"))
    {
        return;
    }

    printf("--------------------------------\n");

    if (!print_element(xml, "test_root/test3/element_3_name"))
    {
        return;
    }

    printf("--------------------------------\n");

    std::string xml_content;
    if (!xml.get_content(xml_content))
    {
        printf("get_content failed\n");
        return;
    }
    printf("content:\n%s\n", xml_content.c_str());

    printf("--------------------------------\n");
}

void test_base_xml(void)
{
    Xml xml;
    test_load_xml(xml);
    printf("   ********   \n");
    test_parse_xml(xml);
    xml.clear();
}
