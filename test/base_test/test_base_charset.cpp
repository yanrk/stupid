#include "test_base.h"
#include "base/charset/charset.h"

USING_NAMESPACE_STUPID_BASE

static void test_base_charset_1(void)
{
    do
    {
        /* test 1 */

        std::string ansi_str1 = "中国rayvision";
        std::string utf8_str1 = CharacterSet(ansi_str1).utfx(UTFX::UTF8);
        std::string utf16_str1 = CharacterSet(ansi_str1).utfx(UTFX::UTF16);
        std::string utf32_str1 = CharacterSet(ansi_str1).utfx(UTFX::UTF32);
        std::wstring unicode_str1 = CharacterSet(ansi_str1).wstr();

        if (CharacterSet(ansi_str1).str() != ansi_str1)
        {
            break;
        }

        /* test 2 */

        std::wstring unicode_str2(unicode_str1);
        std::string utf8_str2 = CharacterSet(unicode_str2).utfx(UTFX::UTF8);
        std::string utf16_str2 = CharacterSet(unicode_str2).utfx(UTFX::UTF16);
        std::string utf32_str2 = CharacterSet(unicode_str2).utfx(UTFX::UTF32);
        std::string ansi_str2 = CharacterSet(unicode_str2).str();

        if (CharacterSet(unicode_str2).wstr() != unicode_str2)
        {
            break;
        }

        if (ansi_str2 != ansi_str1)
        {
            break;
        }

        if (utf8_str2 != utf8_str1)
        {
            break;
        }

        if (utf16_str2 != utf16_str1)
        {
            break;
        }

        if (utf32_str2 != utf32_str1)
        {
            break;
        }

        if (unicode_str2 != unicode_str1)
        {
            break;
        }

        /* test 3 */

        std::string utf8_str3(utf8_str1);
        std::string utf16_str3 = CharacterSet(utf8_str3, UTFX::UTF8).utfx(UTFX::UTF16);
        std::string utf32_str3 = CharacterSet(utf8_str3, UTFX::UTF8).utfx(UTFX::UTF32);
        std::string ansi_str3 = CharacterSet(utf8_str3, UTFX::UTF8).str();
        std::wstring unicode_str3 = CharacterSet(utf8_str3, UTFX::UTF8).wstr();

        if (CharacterSet(utf8_str3, UTFX::UTF8).utfx(UTFX::UTF8) != utf8_str3)
        {
            break;
        }

        if (ansi_str3 != ansi_str1)
        {
            break;
        }

        if (utf8_str3 != utf8_str1)
        {
            break;
        }

        if (utf16_str3 != utf16_str1)
        {
            break;
        }

        if (utf32_str3 != utf32_str1)
        {
            break;
        }

        if (unicode_str3 != unicode_str1)
        {
            break;
        }

        /* test 4 */

        std::string utf16_str4(utf16_str1);
        std::string utf8_str4 = CharacterSet(utf16_str4, UTFX::UTF16).utfx(UTFX::UTF8);
        std::string utf32_str4 = CharacterSet(utf16_str4, UTFX::UTF16).utfx(UTFX::UTF32);
        std::string ansi_str4 = CharacterSet(utf16_str4, UTFX::UTF16).str();
        std::wstring unicode_str4 = CharacterSet(utf16_str4, UTFX::UTF16).wstr();

        if (CharacterSet(utf16_str4, UTFX::UTF16).utfx(UTFX::UTF16) != utf16_str4)
        {
            break;
        }

        if (ansi_str4 != ansi_str1)
        {
            break;
        }

        if (utf8_str4 != utf8_str1)
        {
            break;
        }

        if (utf16_str4 != utf16_str1)
        {
            break;
        }

        if (utf32_str4 != utf32_str1)
        {
            break;
        }

        if (unicode_str4 != unicode_str1)
        {
            break;
        }

        /* test 5 */

        std::string utf32_str5(utf32_str1);
        std::string utf8_str5 = CharacterSet(utf32_str5, UTFX::UTF32).utfx(UTFX::UTF8);
        std::string utf16_str5 = CharacterSet(utf32_str5, UTFX::UTF32).utfx(UTFX::UTF16);
        std::string ansi_str5 = CharacterSet(utf32_str5, UTFX::UTF32).str();
        std::wstring unicode_str5 = CharacterSet(utf32_str5, UTFX::UTF32).wstr();

        if (CharacterSet(utf32_str5, UTFX::UTF32).utfx(UTFX::UTF32) != utf32_str5)
        {
            break;
        }

        if (ansi_str5 != ansi_str1)
        {
            break;
        }

        if (utf8_str5 != utf8_str1)
        {
            break;
        }

        if (utf16_str5 != utf16_str1)
        {
            break;
        }

        if (utf32_str5 != utf32_str1)
        {
            break;
        }

        if (unicode_str5 != unicode_str1)
        {
            break;
        }

        std::cout << "test CharacterSet 1 success" << std::endl;

        return;
    } while (false);

    std::cout << "test CharacterSet 1 failure" << std::endl;
}

static void test_base_charset_2(void)
{
    do
    {
        const char * filename = "utf8_store_ansi.txt";

        const std::string src_ansi_data("中国rayvision");
        const std::string src_utf8_data = CharacterSet(src_ansi_data).utfx(UTFX::UTF8);
        std::string dst_utf8_data;
        std::string dst_ansi_data;

        std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
        if (!ofs.is_open())
        {
            break;
        }
        ofs.write(src_utf8_data.c_str(), src_utf8_data.size());
        ofs.close();

        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs.is_open())
        {
            break;
        }
        ifs.seekg(0, std::ios::end);
        dst_utf8_data.resize(static_cast<size_t>(ifs.tellg()));
        ifs.seekg(0, std::ios::beg);
        ifs.read(&dst_utf8_data[0], dst_utf8_data.size());
        ifs.close();
        dst_ansi_data = CharacterSet(dst_utf8_data, UTFX::UTF8).str();

        if (src_utf8_data != dst_utf8_data)
        {
            break;
        }

        if (src_ansi_data != dst_ansi_data)
        {
            break;
        }

        std::cout << "test CharacterSet 2 success" << std::endl;

        return;
    } while (false);

    std::cout << "test CharacterSet 2 failure" << std::endl;
}

static void test_base_charset_3(void)
{
    do
    {
        const char * filename = "utf8_store_unicode.txt";

        const std::wstring src_unicode_data(L"中国rayvision");
        const std::string src_utf8_data = CharacterSet(src_unicode_data).utfx(UTFX::UTF8);
        std::string dst_utf8_data;
        std::wstring dst_unicode_data;

        std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
        if (!ofs.is_open())
        {
            break;
        }
        ofs.write(src_utf8_data.c_str(), src_utf8_data.size());
        ofs.close();

        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs.is_open())
        {
            break;
        }
        ifs.seekg(0, std::ios::end);
        dst_utf8_data.resize(static_cast<size_t>(ifs.tellg()));
        ifs.seekg(0, std::ios::beg);
        ifs.read(&dst_utf8_data[0], dst_utf8_data.size());
        ifs.close();
        dst_unicode_data = CharacterSet(dst_utf8_data, UTFX::UTF8).wstr();

        if (src_utf8_data != dst_utf8_data)
        {
            break;
        }

        if (src_unicode_data != dst_unicode_data)
        {
            break;
        }

        std::cout << "test CharacterSet 3 success" << std::endl;

        return;
    } while (false);

    std::cout << "test CharacterSet 3 failure" << std::endl;
}

void test_base_charset(void)
{
    test_base_charset_1();
    test_base_charset_2();
    test_base_charset_3();
}
