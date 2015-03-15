#include "test_base.h"
#include "base/config/xml.h"

USING_NAMESPACE_STUPID_BASE

static void test_xml_1(void)
{
    const char * document = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<root>"
            "<web_service>"
                "<command_port>9912</command_port>"
                "<manager_port>9915</manager_port>"
            "</web_service>"
            "<node_service>"
                "<port>9999</port>"
            "</node_service>"
            "<name1>data1</name1>"
            "<name2>data2</name2>"
            "<name3>data3</name3>"
        "</root>";

    Xml xml;

    if (!xml.set_document(document))
    {
        assert(false);
    }

    if (!xml.find_element("root"))
    {
        assert(false);
    }

    std::string element_content = xml.get_element_content();
    std::cout << element_content << std::endl;

    std::string child_element_value;
    if (!xml.get_child_element("name2", child_element_value))
    {
        assert(false);
    }
    if (!xml.get_child_element("name1", child_element_value))
    {
        assert(false);
    }
    if (!xml.get_child_element("name3", child_element_value))
    {
        assert(false);
    }

    if (xml.into_element())
    {
        std::string element_value;
        if (!xml.get_element("name2", element_value))
        {
            assert(false);
        }
        if (!xml.get_element("name1", element_value))
        {
            assert(false);
        }
        if (!xml.get_element("name3", element_value))
        {
            assert(false);
        }

        if (xml.find_element("node_service"))
        {
            std::string port;
            if (!xml.get_child_element("port", port))
            {
                assert(false);
            }
        }
        else
        {
            assert(false);
        }

        if (xml.find_element("web_service"))
        {
            std::string port;
            if (!xml.get_child_element("manager_port", port))
            {
                assert(false);
            }
            if (!xml.get_child_element("command_port", port))
            {
                assert(false);
            }
        }
        else
        {
            assert(false);
        }

        if (xml.into_element("node_service"))
        {
            std::string port;
            if (!xml.get_element("port", port))
            {
                assert(false);
            }

            if (!xml.outof_element())
            {
                assert(false);
            }
        }
        else
        {
            assert(false);
        }

        if (xml.into_element("web_service"))
        {
            std::string port;
            if (!xml.get_element("manager_port", port))
            {
                assert(false);
            }
            if (!xml.get_element("command_port", port))
            {
                assert(false);
            }

            if (!xml.outof_element())
            {
                assert(false);
            }
        }
        else
        {
            assert(false);
        }

        if (!xml.outof_element())
        {
            assert(false);
        }
    }
    else
    {
        assert(false);
    }
}

static void test_xml_2(void)
{
    const char * document = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<root name1=\"data1\" name2=\"data2\" name3=\"data3\">"
            "<web_service command_port=\"9912\" manager_port=\"9915\"></web_service>"
            "<node_service port=\"9999\"></node_service>"
        "</root>";

    Xml xml;

    if (!xml.set_document(document))
    {
        assert(false);
    }

    if (!xml.find_element("root"))
    {
        assert(false);
    }

    std::string element_content = xml.get_element_content();
    std::cout << element_content << std::endl;

    std::string attribute_value;
    if (!xml.get_attribute("name2", attribute_value))
    {
        assert(false);
    }
    if (!xml.get_attribute("name1", attribute_value))
    {
        assert(false);
    }
    if (!xml.get_attribute("name3", attribute_value))
    {
        assert(false);
    }

    std::string child_attribute_value;
    if (!xml.get_child_attribute("node_service", "port", child_attribute_value))
    {
        assert(false);
    }
    if (!xml.get_child_attribute("web_service", "command_port", child_attribute_value))
    {
        assert(false);
    }
    if (!xml.get_child_attribute("web_service", "manager_port", child_attribute_value))
    {
        assert(false);
    }

    if (xml.into_element())
    {
        std::string port;
        if (!xml.get_attribute("node_service", "port", port))
        {
            assert(false);
        }
        if (!xml.get_attribute("web_service", "command_port", port))
        {
            assert(false);
        }
        if (!xml.get_attribute("web_service", "manager_port", port))
        {
            assert(false);
        }
        if (!xml.outof_element())
        {
            assert(false);
        }
    }
    else
    {
        assert(false);
    }
}

static void test_xml_3(void)
{
    const char * document = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<node>"
            "<properties>"
                "<property>p1</property>"
                "<property>p2</property>"
                "<property>p3</property>"
            "</properties>"
            "<requirements>"
                "<requirement>r1</requirement>"
                "<requirement>r2</requirement>"
                "<requirement>r3</requirement>"
            "</requirements>"
        "</node>";

    Xml xml;

    if (!xml.set_document(document))
    {
        assert(false);
    }

    if (!xml.into_element("node"))
    {
        assert(false);
    }

    std::list<std::string> property_list;
    if (!xml.get_element_block("properties", "property", false, property_list))
    {
        assert(false);
    }

    std::list<std::string> requirement_list;
    if (!xml.get_element_block("requirements", "requirement", false, requirement_list))
    {
        assert(false);
    }

    if (!xml.outof_element())
    {
        assert(false);
    }
}

static void test_xml_4(void)
{
    const char * document = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<nodeinfo>"
            "<node>"
                "<id>1</id>"
                "<properties>"
                    "<property>p1</property>"
                "</properties>"
                "<requirements>"
                    "<requirement>r1</requirement>"
                "</requirements>"
            "</node>"
            "<node>"
                "<id>2</id>"
                "<properties>"
                    "<property>p1</property>"
                    "<property>p2</property>"
                "</properties>"
                "<requirements>"
                    "<requirement>r1</requirement>"
                    "<requirement>r2</requirement>"
                "</requirements>"
            "</node>"
            "<node>"
                "<id>3</id>"
                "<properties>"
                    "<property>p1</property>"
                    "<property>p2</property>"
                    "<property>p3</property>"
                "</properties>"
                "<requirements>"
                    "<requirement>r1</requirement>"
                    "<requirement>r2</requirement>"
                    "<requirement>r3</requirement>"
                "</requirements>"
            "</node>"
        "</nodeinfo>";

    Xml xml;

    if (!xml.set_document(document))
    {
        assert(false);
    }

    const std::string main_document = xml.get_document();
    std::cout << main_document << std::endl;

    if (!xml.into_element("nodeinfo"))
    {
        assert(false);
    }

    std::string sub_document;
    size_t sub_document_count = 0;
    while (xml.get_sub_document(sub_document))
    {
        ++sub_document_count;
    }
    assert(3 == sub_document_count);

    if (!xml.outof_element())
    {
        assert(false);
    }
}

static void test_xml_5(void)
{
    /*
     * <?xml version="1.0" encoding="UTF-8"?>
     * <root name1="data1" name2="data2" name3="data3">
     *     <web_service command_port="9912" manager_port="9915"></web_service>
     *     <node_service port="9999"></node_service>
     *     <nodeinfo>
     *         <node>
     *             <id>id1</id>
     *             <ip>ip</ip>
     *             <port>port</port>
     *         </node>
     *         <node>
     *             <id>id2</id>
     *             <ip>ip</ip>
     *             <port>port</port>
     *         </node>
     *     </nodeinfo>
     *     <properties>
     *         <property>p1</property>
     *         <property>p2</property>
     *     </properties>
     *     <requirements>
     *         <requirement>r1</requirement>
     *         <requirement>r2</requirement>
     *     </requirements>
     * </root>
     */

    Xml xml;

    if (!xml.add_element("root"))
    {
        assert(false);
    }

    if (!xml.add_attribute("name1", "data1"))
    {
        assert(false);
    }

    if (!xml.add_child_element("node_service"))
    {
        assert(false);
    }
    if (!xml.add_child_attribute("port", "9999"))
    {
        assert(false);
    }

    if (!xml.add_attribute("name2", "data2"))
    {
        assert(false);
    }

    if (!xml.insert_child_element("web_service"))
    {
        assert(false);
    }
    if (!xml.add_child_attribute("command_port", "9912"))
    {
        assert(false);
    }
    if (!xml.add_child_attribute("manager_port", "9915"))
    {
        assert(false);
    }

    if (!xml.add_attribute("name3", "data3"))
    {
        assert(false);
    }

    if (!xml.into_element("root"))
    {
        assert(false);
    }

    if (!xml.add_element("nodeinfo"))
    {
        assert(false);
    }

    if (!xml.add_child_sub_document("<node><id>id2</id><ip>ip</ip><port>port</port></node>"))
    {
        assert(false);
    }
    if (!xml.insert_child_sub_document("<node><id>id1</id><ip>ip</ip><port>port</port></node>"))
    {
        assert(false);
    }

    if (!xml.add_sub_document("<requirements><requirement>r1</requirement><requirement>r2</requirement></requirements>"))
    {
        assert(false);
    }
    if (!xml.insert_sub_document("<properties><property>p1</property><property>p2</property></properties>"))
    {
        assert(false);
    }

    if (!xml.outof_element())
    {
        assert(false);
    }

    xml.insert_sub_document("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"); /* return false but insert sub document successfully */

    const std::string & document = xml.get_document();
    std::cout << document << std::endl;
}

static void test_xml_6(void)
{
    /*
     * <?xml version="1.0" encoding="UTF-8"?>
     * <node>
     *     <properties>
     *         <property>p1</property>
     *         <property>p2</property>
     *         <property>p3</property>
     *     </properties>
     *     <requirements>
     *         <requirement>r1</requirement>
     *         <requirement>r2</requirement>
     *         <requirement>r3</requirement>
     *     </requirements>
     * </node>
     */

    Xml xml;

    if (!xml.set_document("<?xml version=\"1.0\" encoding=\"UTF-8\"?><node/>"))
    {
        assert(false);
    }

    if (!xml.into_element("node"))
    {
        assert(false);
    }

    std::list<std::string> requirement_list;
    requirement_list.push_back("r1");
    requirement_list.push_back("r2");
    requirement_list.push_back("r3");
    if (!xml.add_element_block("requirements", "requirement", false, requirement_list))
    {
        assert(false);
    }

    std::list<std::string> property_list;
    property_list.push_back("p1");
    property_list.push_back("p2");
    property_list.push_back("p3");
    if (!xml.insert_element_block("properties", "property", false, property_list))
    {
        assert(false);
    }

    if (!xml.outof_element())
    {
        assert(false);
    }

    const std::string & document = xml.get_document();
    std::cout << document << std::endl;
}

static void test_xml_7(void)
{
    const char * document = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<root name1=\"data1\" name2=\"data2\" name3=\"data3\">"
            "<web_service command_port=\"9912\" manager_port=\"9915\"></web_service>"
            "<node_service port=\"9999\"></node_service>"
        "</root>";

    Xml xml;

    if (!xml.set_document(document))
    {
        assert(false);
    }

    if (!xml.find_element("root"))
    {
        assert(false);
    }

    if (!xml.into_element())
    {
        assert(false);
    }

    if (!xml.remove_element("web_service"))
    {
        assert(false);
    }

    if (!xml.outof_element())
    {
        assert(false);
    }

    const std::string & document1 = xml.get_document();
    std::cout << document1 << std::endl;

    if (!xml.remove_child_element("node_service"))
    {
        assert(false);
    }

    const std::string & document2 = xml.get_document();
    std::cout << document2 << std::endl;

    if (!xml.remove_element())
    {
        assert(false);
    }

    const std::string & document3 = xml.get_document();
    std::cout << document3 << std::endl;
}

static void test_xml_8(void)
{
    const char * document = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<root name1=\"data1\" name2=\"data2\" name3=\"data3\">"
            "<web_service command_port=\"9912\" manager_port=\"9915\"></web_service>"
            "<node_service port=\"9999\"></node_service>"
        "</root>";

    Xml xml;

    if (!xml.set_document(document))
    {
        assert(false);
    }

    if (!xml.remove_element("root"))
    {
        assert(false);
    }

    const std::string & new_document = xml.get_document();
    std::cout << new_document << std::endl;
}

static void test_xml_9(void)
{
    /*
     * <root name1="data1" name2="data2" name3="data3">
     *     <web_service command_port="9912" manager_port="9915"></web_service>
     *     <node_service port="9999"></node_service>
     * </root>
     */

    Xml xml;

    if (!xml.set_element("root"))
    {
        assert(false);
    }

    if (!xml.set_attribute("root", "name1", "data1"))
    {
        assert(false);
    }
    if (!xml.set_attribute("root", "name2", "data2"))
    {
        assert(false);
    }
    if (!xml.set_attribute("root", "name3", "data3"))
    {
        assert(false);
    }

    if (!xml.into_element())
    {
        assert(false);
    }

    if (!xml.set_element("web_service"))
    {
        assert(false);
    }
    if (!xml.set_attribute("web_service", "command_port", "9912"))
    {
        assert(false);
    }
    if (!xml.set_attribute("web_service", "manager_port", "9915"))
    {
        assert(false);
    }

    if (!xml.outof_element())
    {
        assert(false);
    }

    if (!xml.set_child_element("node_service"))
    {
        assert(false);
    }

    if (!xml.set_child_attribute("node_service", "port", "9999"))
    {
        assert(false);
    }

    const std::string & document = xml.get_document();
    std::cout << document << std::endl;
}

static void test_xml_10(void)
{
    /*
     * <?xml version="1.0" encoding="UTF-8"?>
     * <root name1="data1" name2="data2" name3="data3">
     *     <web_service command_port="9912" manager_port="9915"></web_service>
     *     <node_service port="9999"></node_service>
     * </root>
     */

    Xml xml;

    if (!xml.set_document("<?xml version=\"1.0\" encoding=\"UTF-8\"?><root name1=\"data1\" name2=\"data2\" name3=\"data3\"></root>"))
    {
        assert(false);
    }

    if (!xml.set_element_content("root", "<web_service command_port=\"9912\" manager_port=\"9915\"></web_service><node_service port=\"9999\"></node_service>"))
    {
        assert(false);
    }

    const std::string & document = xml.get_document();
    std::cout << document << std::endl;
}

static void test_xml_11(void)
{
    Xml xml;

    if (!xml.load("./test.xml"))
    {
        assert(false);
    }

    const std::string & document1 = xml.get_document();
    std::cout << document1 << std::endl;

    /*
     * <?xml version="1.0" encoding="UTF-8"?>
     * <root name1="d1" name2="d2" name3="d3">
     *     <web_service command_port="10001" manager_port="10002">service1</web_service>
     *     <node_service port="11111">service2</node_service>
     * </root>
     */

    if (!xml.set_attribute("root", "name1", "d1"))
    {
        assert(false);
    }
    if (!xml.set_attribute("root", "name2", "d2"))
    {
        assert(false);
    }
    if (!xml.set_attribute("root", "name3", "d3"))
    {
        assert(false);
    }

    if (!xml.into_element("root"))
    {
        assert(false);
    }

    if (!xml.set_element("web_service", "service1"))
    {
        assert(false);
    }
    if (!xml.set_attribute("web_service", "command_port", "10001"))
    {
        assert(false);
    }
    if (!xml.set_attribute("web_service", "manager_port", "10002"))
    {
        assert(false);
    }

    if (!xml.outof_element())
    {
        assert(false);
    }

    if (!xml.set_child_element("node_service", "service2"))
    {
        assert(false);
    }

    if (!xml.set_child_attribute("node_service", "port", "11111"))
    {
        assert(false);
    }

    const std::string & document2 = xml.get_document();
    std::cout << document2 << std::endl;

    if (!xml.save("./test.xml"))
    {
        assert(false);
    }
}

void test_base_xml(void)
{
    test_xml_1();
    test_xml_2();
    test_xml_3();
    test_xml_4();
    test_xml_5();
    test_xml_6();
    test_xml_7();
    test_xml_8();
    test_xml_9();
    test_xml_10();
    test_xml_11();
}
