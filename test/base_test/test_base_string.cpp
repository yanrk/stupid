#include <list>
#include <vector>
#include "test_base.h"
#include "base/string/string.h"

USING_NAMESPACE_STUPID_BASE

static void test_base_string_1(void)
{
    std::cout << "test string format" << std::endl;
    {
        const char fmt[] = "12345678901234567890123456789012";
        char buf[29];
        int size = sizeof(buf)/sizeof(buf[0]);
        stupid_snprintf(buf, size, fmt);
        printf("%d - %s\n", size, buf);
    }

    {
        const char fmt[] = "12345678901234567890123456789012";
        char buf[30];
        int size = sizeof(buf)/sizeof(buf[0]);
        stupid_snprintf(buf, size, fmt);
        printf("%d - %s\n", size, buf);
    }

    {
        const char fmt[] = "12345678901234567890123456789012";
        char buf[31];
        int size = sizeof(buf)/sizeof(buf[0]);
        stupid_snprintf(buf, size, fmt);
        printf("%d - %s\n", size, buf);
    }

    {
        const char fmt[] = "12345678901234567890123456789012";
        char buf[32];
        int size = sizeof(buf)/sizeof(buf[0]);
        stupid_snprintf(buf, size, fmt);
        printf("%d - %s\n", size, buf);
    }

    {
        const char fmt[] = "12345678901234567890123456789012";
        char buf[33];
        int size = sizeof(buf)/sizeof(buf[0]);
        stupid_snprintf(buf, size, fmt);
        printf("%d - %s\n", size, buf);
    }

    {
        const char fmt[] = "12345678901234567890123456789012";
        char buf[34];
        int size = sizeof(buf)/sizeof(buf[0]);
        stupid_snprintf(buf, size, fmt);
        printf("%d - %s\n", size, buf);
    }

    {
        const char fmt[] = "12345678901234567890123456789012";
        char buf[35];
        int size = sizeof(buf)/sizeof(buf[0]);
        stupid_snprintf(buf, size, fmt);
        printf("%d - %s\n", size, buf);
    }

    {
        const char fmt[] = "12345678901234567890123456789012";
        char buf[36];
        int size = sizeof(buf)/sizeof(buf[0]);
        stupid_snprintf(buf, size, fmt);
        printf("%d - %s\n", size, buf);
    }
    std::cout << std::endl;
}

static void test_base_string_2(void)
{
    std::cout << "compare str1 and str2 ignore case" << std::endl;
    {
        const char * str1 = "ABC";
        const char * str2 = "abb";
        const char * str3 = "abd";
        const char * str4 = "abc";
        const char * str5 = "abcd";

        std::cout << "compare all string" << std::endl;

        std::cout << str1 << " V.S " << str2 << " : " 
                  << stupid_strcmp_ignore_case(str1, str2) << std::endl;
        std::cout << str1 << " V.S " << str3 << " : " 
                  << stupid_strcmp_ignore_case(str1, str3) << std::endl;
        std::cout << str1 << " V.S " << str4 << " : " 
                  << stupid_strcmp_ignore_case(str1, str4) << std::endl;
        std::cout << str1 << " V.S " << str5 << " : " 
                  << stupid_strcmp_ignore_case(str1, str5) << std::endl;

        std::cout << "compare 3 character" << std::endl;

        std::cout << str1 << " V.S " << str2 << " : " 
                  << stupid_strncmp_ignore_case(str1, str2, 3) << std::endl;
        std::cout << str1 << " V.S " << str3 << " : " 
                  << stupid_strncmp_ignore_case(str1, str3, 3) << std::endl;
        std::cout << str1 << " V.S " << str4 << " : " 
                  << stupid_strncmp_ignore_case(str1, str4, 3) << std::endl;
        std::cout << str1 << " V.S " << str5 << " : " 
                  << stupid_strncmp_ignore_case(str1, str5, 3) << std::endl;
    }
    std::cout << std::endl;
}

static void test_base_string_3(void)
{
    std::cout << "test c string to type" << std::endl;
    {
        const char * str = "A";
        char val = ' ';
        stupid_string_to_char(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        const char * str = "123";
        short val = 0;
        stupid_string_to_short(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        const char * str = "1234";
        int val = 0;
        stupid_string_to_int(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        const char * str = "123456789";
        long val = 0;
        stupid_string_to_long(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        const char * str = "123.456";
        float val = 0.0f;
        stupid_string_to_float(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        const char * str = "123.456789";
        double val = 0.0;
        stupid_string_to_double(str, val);
        std::cout << str << " : " << val << std::endl;
    }
    std::cout << std::endl;

    std::cout << "test c++ string to type" << std::endl;
    {
        std::string str("A");
        char val = ' ';
        stupid_string_to_char(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        std::string str("123");
        short val = 0;
        stupid_string_to_short(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        std::string str("1234");
        int val = 0;
        stupid_string_to_int(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        std::string str("123456789");
        long val = 0;
        stupid_string_to_long(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        std::string str("123.456");
        float val = 0.0f;
        stupid_string_to_float(str, val);
        std::cout << str << " : " << val << std::endl;
    }

    {
        std::string str("123.456789");
        double val = 0.0;
        stupid_string_to_double(str, val);
        std::cout << str << " : " << val << std::endl;
    }
    std::cout << std::endl;

    std::cout << "test type to c string" << std::endl;
    {
        const int size = 64;
        char str[size];
        char val = 'A';
        stupid_char_to_string(val, str, size);
        std::cout << val << " : " << str << std::endl;
    }

    {
        const int size = 64;
        char str[size];
        short val = 123;
        stupid_short_to_string(val, str, size);
        std::cout << val << " : " << str << std::endl;
    }

    {
        const int size = 64;
        char str[size];
        int val = 1234;
        stupid_int_to_string(val, str, size);
        std::cout << val << " : " << str << std::endl;
    }

    {
        const int size = 64;
        char str[size];
        long val = 123456789;
        stupid_long_to_string(val, str, size);
        std::cout << val << " : " << str << std::endl;
    }

    {
        const int size = 64;
        char str[size];
        float val = 123.456f;
        stupid_float_to_string(val, str, size);
        std::cout << val << " : " << str << std::endl;
    }

    {
        const int size = 64;
        char str[size];
        double val = 123.456789;
        stupid_double_to_string(val, str, size);
        std::cout << val << " : " << str << std::endl;
    }
    std::cout << std::endl;

    std::cout << "test type to c++ string" << std::endl;
    {
        std::string str;
        char val = 'A';
        stupid_char_to_string(val, str);
        std::cout << val << " : " << str << std::endl;
    }

    {
        std::string str;
        short val = 123;
        stupid_short_to_string(val, str);
        std::cout << val << " : " << str << std::endl;
    }

    {
        std::string str;
        int val = 1234;
        stupid_int_to_string(val, str);
        std::cout << val << " : " << str << std::endl;
    }

    {
        std::string str;
        long val = 123456789;
        stupid_long_to_string(val, str);
        std::cout << val << " : " << str << std::endl;
    }

    {
        std::string str;
        float val = 123.456f;
        stupid_float_to_string(val, str);
        std::cout << val << " : " << str << std::endl;
    }

    {
        std::string str;
        double val = 123.456789;
        stupid_double_to_string(val, str);
        std::cout << val << " : " << str << std::endl;
    }
    std::cout << std::endl;
}

static void test_base_string_4(void)
{
    std::cout.setf(std::ios::fixed, std::ios::floatfield);

    std::cout << std::endl;
    std::cout << "test type to string" << std::endl;

    {
        bool b1 = false;
        std::string str;
        stupid_type_to_string(b1, str);
        std::cout << b1 << " : " << str << std::endl;
    }

    {
        bool b2 = true;
        std::string str;
        stupid_type_to_string(b2, str);
        std::cout << b2 << " : " << str << std::endl;
    }

    {
        char c = 'a';
        std::string str;
        stupid_type_to_string(c, str);
        std::cout << c << " : " << str << std::endl;
    }

    {
        short s = 123;
        std::string str;
        stupid_type_to_string(s, str);
        std::cout << s << " : " << str << std::endl;
    }

    {
        int i = 13579;
        std::string str;
        stupid_type_to_string(i, str);
        std::cout << i << " : " << str << std::endl;
    }

    {
        int l = 123456789;
        std::string str;
        stupid_type_to_string(l, str);
        std::cout << l << " : " << str << std::endl;
    }

    {
        float f = 123.4567f;
        std::string str;
        stupid_type_to_string(f, str);
        std::cout << f << " : " << str << std::endl;
    }

    {
        double d = 123456.1234567;
        std::string str;
        stupid_type_to_string(d, str);
        std::cout << d << " : " << str << std::endl;
    }

    std::cout << std::endl;
    std::cout << "test string to type" << std::endl;

    {
        std::string str("false");
        bool b1 = true;
        stupid_string_to_type(str, b1);
        std::cout << str << " : " << b1 << std::endl;
    }

    {
        std::string str("true");
        bool b2 = false;
        stupid_string_to_type(str, b2);
        std::cout << str << " : " << b2 << std::endl;
    }

    {
        std::string str("a");
        char c = ' ';
        stupid_string_to_type(str, c);
        std::cout << str << " : " << c << std::endl;
    }

    {
        std::string str("123");
        short s = 0;
        stupid_string_to_type(str, s);
        std::cout << str << " : " << s << std::endl;
    }

    {
        std::string str("13579");
        int i = 0;
        stupid_string_to_type(str, i);
        std::cout << str << " : " << i << std::endl;
    }

    {
        std::string str("123456789");
        int l = 0;
        stupid_string_to_type(str, l);
        std::cout << str << " : " << l << std::endl;
    }

    {
        std::string str("123.4567");
        float f = 0.0f;
        stupid_string_to_type(str, f);
        std::cout << str << " : " << f << std::endl;
    }

    {
        std::string str("123456.1234567");
        double d = 0.0;
        stupid_string_to_type(str, d);
        std::cout << str << " : " << d << std::endl;
    }

    std::cout << std::endl;

    std::cout.unsetf(std::ios::floatfield);
}

static void test_base_string_5(void)
{
    std::cout << "test string trim and simplify" << std::endl;

    {
        std::string str(" \r\n\v\t\f123   ");
        std::cout << "\" \\r\\n\\v\\t\\f123   \", after trim-head is:" << std::endl;
        stupid_string_trim_head(str);
        std::cout << "\t\"" << str << "\"" << std::endl;
    }

    {
        std::string str("   123\t\v\r\n\f ");
        std::cout << "\"   123\\t\\v\\r\\n\\f \", after trim-tail is:" << std::endl;
        stupid_string_trim_tail(str);
        std::cout << "\t\"" << str << "\"" << std::endl;
    }

    {
        std::string str(" \r\n\v\t\f123\t\v\r\n\f ");
        std::cout << "\" \\r\\n\\v\\t\\f123\\t\\v\\r\\n\\f \", after trim is:" << std::endl;
        stupid_string_trim(str);
        std::cout << "\t\"" << str << "\"" << std::endl;
    }

    {
        std::string str(" \r\n\v\t\f123 \r\n456\t\v\f 789\t\v\r\n\f ");
        std::cout << "\" \\r\\n\\v\\t\\f123 \\r\\n456\\t\\v\\f 789\\t\\v\\r\\n\\f \", after default simplify is:" << std::endl;
        stupid_string_simplify(str);
        std::cout << "\t\"" << str << "\"" << std::endl;
    }

    {
        std::string str(" \r\n\v\t\f123 \r\n456\t\v\f 789\t\v\r\n\f ");
        std::cout << "\" \\r\\n\\v\\t\\f123 \\r\\n456\\t\\v\\f 789\\t\\v\\r\\n\\f \", after simplify with ',' is:" << std::endl;
        stupid_string_simplify(str, g_blank_character_set, ',');
        std::cout << "\t\"" << str << "\"" << std::endl;
    }

    {
        std::cout << std::endl;

        std::list<std::string> all_set;
        all_set.push_back("A");
        all_set.push_back("B");
        all_set.push_back("C");
        all_set.push_back("D");
        all_set.push_back("E");
        std::vector<std::string> part_set;
        part_set.push_back("C");
        part_set.push_back("A");
        part_set.push_back("E");
        part_set.push_back("A");
        part_set.push_back("C");

        if (Stupid::Base::stupid_includes(all_set.begin(), all_set.end(), part_set.begin(), part_set.end()))
        {
            std::cout << "stupid_includes work ok" << std::endl;
        }
        else
        {
            std::cout << "stupid_includes do not work" << std::endl;
        }

        if (!Stupid::Base::stupid_includes(part_set.begin(), part_set.end(), all_set.begin(), all_set.end()))
        {
            std::cout << "stupid_includes work ok" << std::endl;
        }
        else
        {
            std::cout << "stupid_includes do not work" << std::endl;
        }

        std::cout << std::endl;
    }

    {
        std::list<std::string> src_element_set;
        src_element_set.push_back("A");
        src_element_set.push_back("B");
        src_element_set.push_back("C");
        src_element_set.push_back("D");
        src_element_set.push_back("E");
        std::cout << "source element set:" << std::endl;
        for (std::list<std::string>::const_iterator iter = src_element_set.begin(); src_element_set.end() != iter; ++iter)
        {
            std::cout << "\t" << *iter << std::endl;
        }
        std::cout << std::endl;

        std::string elements;
        Stupid::Base::stupid_piece_together(src_element_set.begin(), src_element_set.end(), ",", elements);
        std::cout << "piece_together result: " << elements << std::endl;

        std::list<std::string> dst_element_set;
        Stupid::Base::stupid_split_piece(elements, ",", true, dst_element_set);
        std::cout << "split_piece result:" << std::endl;
        for (std::list<std::string>::const_iterator iter = dst_element_set.begin(); dst_element_set.end() != iter; ++iter)
        {
            std::cout << "\t" << *iter << std::endl;
        }
        std::cout << std::endl;
    }

    {
        const char * command_line = " 1st \" 2nd 2nd \" \'3rd\' 4th 5th ";
        std::cout << "command line: {" << command_line << "}" << std::endl << std::endl;

        std::list<std::string> result1;
        if (Stupid::Base::stupid_split_command_line(command_line, result1, " \'\"", false))
        {
            std::cout << "split_command_line result:(not trim delimiter)" << std::endl;
            for (std::list<std::string>::const_iterator iter = result1.begin(); result1.end() != iter; ++iter)
            {
                std::cout << "\t{" << *iter << "}" << std::endl;
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "stupid_split_command_line failed" << std::endl;
        }

        std::list<std::string> result2;
        if (Stupid::Base::stupid_split_command_line(command_line, result2, " \'\"", true))
        {
            std::cout << "split_command_line result:(trim delimiter)" << std::endl;
            for (std::list<std::string>::const_iterator iter = result2.begin(); result2.end() != iter; ++iter)
            {
                std::cout << "\t{" << *iter << "}" << std::endl;
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "stupid_split_command_line failed" << std::endl;
        }
    }

    std::cout << std::endl;
}

static void test_base_string_6(void)
{
    std::cout << "test string to type and type to string" << std::endl;

    std::list<std::string> src_list;
    src_list.push_back("123");
    src_list.push_back("456");
    src_list.push_back("789");

    std::list<int> int_list;
    stupid_string_to_type(src_list, int_list);

    std::list<std::string> dst_list;
    stupid_type_to_string(int_list, dst_list);

    if (stupid_includes(src_list.begin(), src_list.end(), dst_list.begin(), dst_list.end()) && 
        stupid_includes(dst_list.begin(), dst_list.end(), src_list.begin(), src_list.end()))
    {
        std::cout << "success" << std::endl;
    }
    else
    {
        std::cout << "failure" << std::endl;
    }
    std::cout << std::endl;
}

static void test_base_string_7()
{
    /*
    {
        const std::string src("hello");
        char dst[16] = { 0 };
        stupid_string_to_type(src, dst); // string-to-pointer has been disabled
    }

    {
        std::string src("hello");
        char dst[16] = { 0 };
        stupid_string_to_type(src, dst); // string-to-pointer has been disabled
    }

    {
        const char src[16] = "hello";
        std::string dst;
        stupid_type_to_string(src, dst); // pointer-to-string has been disabled
    }

    {
        char src[16] = "hello";
        std::string dst;
        stupid_type_to_string(src, dst); // pointer-to-string has been disabled
    }
    */
}

void test_base_string(void)
{
    test_base_string_1();
    test_base_string_2();
    test_base_string_3();
    test_base_string_4();
    test_base_string_5();
    test_base_string_6();
    test_base_string_7();
}
