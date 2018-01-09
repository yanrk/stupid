/********************************************************
 * Description : character set conversion class
 * History     : Nana C++ Library
 * Author      : jinhao
 * Sources     : github.com/kirbyfan64/nana.git
 * Email       : cnjinhao@hotmail.com
 * WebSide     : www.nanapro.org/zh-cn/
 * Modified By : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * Data        : 2014-10-31 09:51:45
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <clocale>
#endif // _MSC_VER

#include <cwchar>
#include <utility>
#include "base/charset/charset.h"

NAMESPACE_STUPID_BASE_BEGIN

#ifndef _MSC_VER
static void locale_init()
{
    static bool initialized = false;
    if (!initialized)
    {
        /*
         * only set the c library locale
         */
        std::setlocale(LC_CTYPE, "");
        initialized = true;
    }
}
#endif // _MSC_VER

static bool host_is_little_endian()
{
    static union
    {
        unsigned short us;
        unsigned char  uc[sizeof(unsigned short)];
    } un;
    un.us = 0x0001;

    return(0x01 == un.uc[0]);
}

static bool is_little_endian(ENDIAN::TYPE endian_type)
{
    switch (endian_type)
    {
        case ENDIAN::LITTLE:
        {
            return(true);
        }
        case ENDIAN::BIG:
        {
            return(false);
        }
        default:
        {
            return(host_is_little_endian());
        }
    }
}

static bool unicode_to_ansi(const wchar_t * unicode_str, std::string & ansi_str)
{
    ansi_str.clear();

    if (nullptr == unicode_str || L'\0' == unicode_str[0])
    {
        return(true);
    }

#ifdef _MSC_VER
    int bytes_count = ::WideCharToMultiByte(CP_ACP, 0, unicode_str, -1, nullptr, 0, nullptr, nullptr);
    if (bytes_count > 1)
    {
        ansi_str.resize(bytes_count - 1);
        ::WideCharToMultiByte(CP_ACP, 0, unicode_str, -1, &ansi_str[0], bytes_count - 1, nullptr, nullptr);
    }
#else
    locale_init();
    std::mbstate_t mbstate = std::mbstate_t();
    std::size_t bytes_count = std::wcsrtombs(nullptr, &unicode_str, 0, &mbstate);
    if (static_cast<std::size_t>(-1) == bytes_count)
    {
        return(false);
    }
    else if (0 != bytes_count)
    {
        ansi_str.resize(bytes_count);
        std::wcsrtombs(&ansi_str[0], &unicode_str, bytes_count, &mbstate);
    }
#endif // _MSC_VER

    return(true);
}

static bool ansi_to_unicode(const char * ansi_str, std::wstring & unicode_str)
{
    unicode_str.clear();

    if (nullptr == ansi_str || '\0' == ansi_str[0])
    {
        return(true);
    }

#ifdef _MSC_VER
    int chars_count = ::MultiByteToWideChar(CP_ACP, 0, ansi_str, -1, nullptr, 0);
    if (chars_count > 1)
    {
        unicode_str.resize(chars_count - 1);
        ::MultiByteToWideChar(CP_ACP, 0, ansi_str, -1, &unicode_str[0], chars_count - 1);
    }
#else
    locale_init();
    std::mbstate_t mbstate = std::mbstate_t();
    std::size_t chars_count = std::mbsrtowcs(nullptr, &ansi_str, 0, &mbstate);
    if (static_cast<std::size_t>(-1) == chars_count)
    {
        return(false);
    }
    else if (0 != chars_count)
    {
        unicode_str.resize(chars_count);
        std::mbsrtowcs(&unicode_str[0], &ansi_str, chars_count, &mbstate);
    }
#endif // _MSC_VER

    return(true);
}

static bool ansi_to_utfx(const char * ansi_str, std::string & utf_str)
{
    utf_str.clear();

    if (nullptr == ansi_str || '\0' == ansi_str[0])
    {
        return(true);
    }

#ifdef _MSC_VER
    int chars_count = ::MultiByteToWideChar(CP_ACP, 0, ansi_str, -1, nullptr, 0);
    if (chars_count > 1)
    {
        utf_str.resize(sizeof(wchar_t) * (chars_count - 1));
        ::MultiByteToWideChar(CP_ACP, 0, ansi_str, -1, reinterpret_cast<wchar_t *>(&utf_str[0]), chars_count - 1);
    }
#else
    locale_init();
    std::mbstate_t mbstate = std::mbstate_t();
    std::size_t chars_count = std::mbsrtowcs(nullptr, &ansi_str, 0, &mbstate);
    if (static_cast<std::size_t>(-1) == chars_count)
    {
        return(false);
    }
    else if (0 != chars_count)
    {
        utf_str.resize(sizeof(wchar_t) * chars_count);
        std::mbsrtowcs(reinterpret_cast<wchar_t *>(&utf_str[0]), &ansi_str, chars_count, &mbstate);
    }
#endif // _MSC_VER

    return(true);
}

static unsigned long get_utf8char(const unsigned char *& bytes, const unsigned char * end)
{
    unsigned long utf8_value = 0L;
    if (bytes != end)
    {
        if (bytes[0] < 0x80)
        {
            utf8_value = bytes[0];
            bytes += 1;
        }
        else if (bytes[0] < 0xC0)
        {
            utf8_value = 0L;
            bytes = end;
        }
        else if (bytes[0] < 0xE0 && (bytes + 1 <= end))
        {
            utf8_value = ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
            bytes += 2;
        }
        else if (bytes[0] < 0xF0 && (bytes + 2 <= end))
        {
            utf8_value = ((((bytes[0] & 0x0F) << 6) | (bytes[1] & 0x3F)) << 6) | (bytes[2] & 0x3F);
            bytes += 3;
        }
        else if (bytes[0] < 0x1F && (bytes + 3 <= end))
        {
            utf8_value = ((((((bytes[0] & 0x07) << 6) | (bytes[1] & 0x3F)) << 6) | (bytes[2] & 0x3F)) << 6) | (bytes[3] & 0x3F);
            bytes += 4;
        }
        else
        {
            utf8_value = 0L;
            bytes = end;
        }
    }
    return(utf8_value);
}

static unsigned long get_utf16char(const unsigned char *& bytes, const unsigned char * end, bool src_is_little_endian)
{
    unsigned long utf16_value = 0L;
    if (src_is_little_endian)
    {
        if ((bytes + 4 <= end) && ((bytes[1] & 0xFC) == 0xD8))
        {
            unsigned long ch0 = bytes[0] | (bytes[1] << 8);
            unsigned long ch1 = bytes[2] | (bytes[3] << 8);
            utf16_value = (((ch0 & 0x03FF) << 10) | (ch1 & 0x03FF)) + 0x010000;
            bytes += 4;
        }
        else if (bytes + 2 <= end)
        {
            utf16_value = bytes[0] | (bytes[1] << 8);
            bytes += 2;
        }
        else
        {
            bytes = end;
        }
    }
    else
    {
        if ((bytes + 4 <= end) && ((bytes[0] & 0xFC) == 0xD8))
        {
            unsigned long ch0 = (bytes[0] << 8) | bytes[1];
            unsigned long ch1 = (bytes[2] << 8) | bytes[3];
            utf16_value = (((ch0 & 0x03FF) << 10) | (ch1 & 0x03FF)) + 0x010000;
            bytes += 4;
        }
        else if (bytes + 2 <= end)
        {
            utf16_value = (bytes[0] << 8) | bytes[1];
            bytes += 2;
        }
        else
        {
            bytes = end;
        }
    }
    return(utf16_value);
}

static unsigned long get_utf32char(const unsigned char *& bytes, const unsigned char * end, bool src_is_little_endian)
{
    unsigned long utf32_value = 0L;
    if (bytes + 4 <= end)
    {
        if (src_is_little_endian)
        {
            utf32_value = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
        }
        else
        {
            utf32_value = bytes[3] | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);
        }
        bytes += 4;
    }
    else
    {
        bytes = end;
    }
    return(utf32_value);
}

static void put_utf8char(unsigned long utf8_value, std::string & utf8_str)
{
    if (utf8_value < 0x80)
    {
        utf8_str += static_cast<char>(utf8_value);
    }
    else if (utf8_value < 0x0800)
    {
        utf8_str += static_cast<char>(0xC0 | ((utf8_value >> 6)));
        utf8_str += static_cast<char>(0x80 | ((utf8_value & 0x3F)));
    }
    else if (utf8_value < 0x010000)
    {
        utf8_str += static_cast<char>(0xE0 | ((utf8_value >> 12)));
        utf8_str += static_cast<char>(0x80 | ((utf8_value >> 6) & 0x3F));
        utf8_str += static_cast<char>(0x80 | ((utf8_value & 0x3F)));
    }
    else
    {
        utf8_str += static_cast<char>(0xF0 | ((utf8_value >> 18)));
        utf8_str += static_cast<char>(0x80 | ((utf8_value >> 12) & 0x3F));
        utf8_str += static_cast<char>(0x80 | ((utf8_value >> 6) & 0x3F));
        utf8_str += static_cast<char>(0x80 | ((utf8_value & 0x3F)));
    }
}

static void put_utf16char(unsigned long utf16_value, bool dst_is_little_endian, std::string & utf16_str)
{
    if (dst_is_little_endian)
    {
        if ((utf16_value <= 0xD7FF) || (utf16_value >= 0xE000 && utf16_value <= 0xFFFF))
        {
            utf16_str += static_cast<char>((utf16_value & 0xFF));
            utf16_str += static_cast<char>((utf16_value & 0xFF00) >> 8);
        }
        else if (utf16_value >= 0x010000 && utf16_value <= 0x10FFFF)
        {
            unsigned long ch0 = (0xD800 | ((utf16_value - 0x010000) >> 10));
            unsigned long ch1 = (0xDC00 | ((utf16_value - 0x010000) & 0x03FF));
            utf16_str += static_cast<char>((ch0 & 0xFF));
            utf16_str += static_cast<char>((ch0 & 0xFF00) >> 8);
            utf16_str += static_cast<char>((ch1 & 0xFF));
            utf16_str += static_cast<char>((ch1 & 0xFF00) >> 8);
        }
    }
    else
    {
        if ((utf16_value <= 0xD7FF) || (utf16_value >= 0xE000 && utf16_value <= 0xFFFF))
        {
            utf16_str += static_cast<char>((utf16_value & 0xFF00) >> 8);
            utf16_str += static_cast<char>((utf16_value & 0xFF));
        }
        else if (utf16_value >= 0x010000 && utf16_value <= 0x10FFFF)
        {
            unsigned long ch0 = (0xD800 | ((utf16_value - 0x010000) >> 10));
            unsigned long ch1 = (0xDC00 | ((utf16_value - 0x010000) & 0x03FF));
            utf16_str += static_cast<char>((ch0 & 0xFF00) >> 8);
            utf16_str += static_cast<char>((ch0 & 0xFF));
            utf16_str += static_cast<char>((ch1 & 0xFF00) >> 8);
            utf16_str += static_cast<char>((ch1 & 0xFF));
        }
    }
}

static void put_utf32char(unsigned long utf32_value, bool dst_is_little_endian, std::string & utf32_str)
{
    if (dst_is_little_endian)
    {
        utf32_str += static_cast<char>((utf32_value & 0xFF));
        utf32_str += static_cast<char>((utf32_value & 0xFF00) >> 8);
        utf32_str += static_cast<char>((utf32_value & 0xFF0000) >> 16);
        utf32_str += static_cast<char>((utf32_value & 0xFF000000) >> 24);
    }
    else
    {
        utf32_str += static_cast<char>((utf32_value & 0xFF000000) >> 24);
        utf32_str += static_cast<char>((utf32_value & 0xFF0000) >> 16);
        utf32_str += static_cast<char>((utf32_value & 0xFF00) >> 8);
        utf32_str += static_cast<char>((utf32_value & 0xFF));
    }
}

static const std::string & utf8_to_utf8(const std::string & utf8_str)
{
    return(utf8_str);
}

static std::string utf8_to_utf16(const std::string & utf8_str, bool dst_is_little_endian)
{
    const unsigned char * bytes = reinterpret_cast<const unsigned char *>(utf8_str.c_str());
    const unsigned char * end = bytes + utf8_str.size();

    std::string utf16_str;

    /*
     * if there is a BOM-(Byte Order Mark), ignore it
     */
    if (utf8_str.size() >= 3)
    {
        if (0xEF == bytes[0] && 0xBB == bytes[1] && 0xBF == bytes[2])
        {
            bytes += 3;
            put_utf16char(0xFEFF, dst_is_little_endian, utf16_str);
        }
    }

    while (bytes != end)
    {
        put_utf16char(get_utf8char(bytes, end), dst_is_little_endian, utf16_str);
    }

    return(utf16_str);
}

static std::string utf8_to_utf32(const std::string & utf8_str, bool dst_is_little_endian)
{
    const unsigned char * bytes = reinterpret_cast<const unsigned char *>(utf8_str.c_str());
    const unsigned char * end = bytes + utf8_str.size();

    std::string utf32_str;

    /*
     * if there is a BOM-(Byte Order Mark), ignore it
     */
    if (utf8_str.size() >= 3)
    {
        if (0xEF == bytes[0] && 0xBB == bytes[1] && 0xBF == bytes[2])
        {
            bytes += 3;
            put_utf32char(0xFEFF, dst_is_little_endian, utf32_str);
        }
    }

    while (bytes != end)
    {
        put_utf32char(get_utf8char(bytes, end), dst_is_little_endian, utf32_str);
    }

    return(utf32_str);
}

static std::string utf16_to_utf8(const std::string & utf16_str, bool src_is_little_endian)
{
    const unsigned char * bytes = reinterpret_cast<const unsigned char *>(utf16_str.c_str());
    const unsigned char * end = bytes + utf16_str.size();

    std::string utf8_str;

    /*
     * if there is a BOM-(Byte Order Mark), ignore it
     */
    if (utf16_str.size() >= 2)
    {
        if (0xFF == bytes[0] && 0xFE == bytes[1])
        {
            bytes += 2;
            src_is_little_endian = true;
            utf8_str += (char)(0xEF);
            utf8_str += (char)(0xBB);
            utf8_str += (char)(0xBF);
        }
        else if (0xFE == bytes[0] && 0xFF == bytes[1])
        {
            bytes += 2;
            src_is_little_endian = false;
            utf8_str += (char)(0xEF);
            utf8_str += (char)(0xBB);
            utf8_str += (char)(0xBF);
        }
    }

    while (bytes != end)
    {
        put_utf8char(get_utf16char(bytes, end, src_is_little_endian), utf8_str);
    }

    return(utf8_str);
}

static std::string utf16_to_utf16(const std::string & src_utf16_str, bool src_is_little_endian, bool dst_is_little_endian)
{
    const unsigned char * bytes = reinterpret_cast<const unsigned char *>(src_utf16_str.c_str());
    const unsigned char * end = bytes + src_utf16_str.size();

    std::string dst_utf16_str;

    /*
     * if there is a BOM-(Byte Order Mark), ignore it
     */
    if (src_utf16_str.size() >= 2)
    {
        if (0xFF == bytes[0] && 0xFE == bytes[1])
        {
            bytes += 2;
            src_is_little_endian = true;
            put_utf16char(0xFEFF, dst_is_little_endian, dst_utf16_str);
        }
        else if (0xFE == bytes[0] && 0xFF == bytes[1])
        {
            bytes += 2;
            src_is_little_endian = false;
            put_utf16char(0xFEFF, dst_is_little_endian, dst_utf16_str);
        }
    }

    while (bytes != end)
    {
        put_utf16char(get_utf16char(bytes, end, src_is_little_endian), dst_is_little_endian, dst_utf16_str);
    }

    return(dst_utf16_str);
}

static std::string utf16_to_utf32(const std::string & utf16_str, bool src_is_little_endian, bool dst_is_little_endian)
{
    const unsigned char * bytes = reinterpret_cast<const unsigned char *>(utf16_str.c_str());
    const unsigned char * end = bytes + utf16_str.size();

    std::string utf32_str;

    /*
     * if there is a BOM-(Byte Order Mark), ignore it
     */
    if (utf16_str.size() >= 2)
    {
        if (0xFF == bytes[0] && 0xFE == bytes[1])
        {
            bytes += 2;
            src_is_little_endian = true;
            put_utf32char(0xFEFF, dst_is_little_endian, utf32_str);
        }
        else if (0xFE == bytes[0] && 0xFF == bytes[1])
        {
            bytes += 2;
            src_is_little_endian = false;
            put_utf32char(0xFEFF, dst_is_little_endian, utf32_str);
        }
    }

    while (bytes != end)
    {
        put_utf32char(get_utf16char(bytes, end, src_is_little_endian), dst_is_little_endian, utf32_str);
    }

    return(utf32_str);
}

static std::string utf32_to_utf8(const std::string & utf32_str, bool src_is_little_endian)
{
    const unsigned char * bytes = reinterpret_cast<const unsigned char*>(utf32_str.c_str());
    const unsigned char * end = bytes + utf32_str.size();

    std::string utf8_str;

    /*
     * if there is a BOM-(Byte Order Mark), ignore it
     */
    if (utf32_str.size() >= 4)
    {
        if (0xFF == bytes[0] && 0xFE == bytes[1] && 0x00 == bytes[2] && 0x00 == bytes[3])
        {
            bytes += 4;
            src_is_little_endian = true;
            utf8_str += (char)(0xEF);
            utf8_str += (char)(0xBB);
            utf8_str += (char)(0xBF);
        }
        else if (0x00 == bytes[0] && 0x00 == bytes[1] && 0xFE == bytes[2] && 0xFF == bytes[3])
        {
            bytes += 4;
            src_is_little_endian = false;
            utf8_str += (char)(0xEF);
            utf8_str += (char)(0xBB);
            utf8_str += (char)(0xBF);
        }
    }

    while (bytes != end)
    {
        put_utf8char(get_utf32char(bytes, end, src_is_little_endian), utf8_str);
    }

    return(utf8_str);
}

static std::string utf32_to_utf16(const std::string & utf32_str, bool src_is_little_endian, bool dst_is_little_endian)
{
    const unsigned char * bytes = reinterpret_cast<const unsigned char*>(utf32_str.c_str());
    const unsigned char * end = bytes + utf32_str.size();

    std::string utf16_str;

    /*
     * if there is a BOM-(Byte Order Mark), ignore it
     */
    if (utf32_str.size() >= 4)
    {
        if (0xFF == bytes[0] && 0xFE == bytes[1] && 0x00 == bytes[2] && 0x00 == bytes[3])
        {
            bytes += 4;
            src_is_little_endian = true;
            put_utf16char(0xFEFF, dst_is_little_endian, utf16_str);
        }
        else if (0x00 == bytes[0] && 0x00 == bytes[1] && 0xFE == bytes[2] && 0xFF == bytes[3])
        {
            bytes += 4;
            src_is_little_endian = false;
            put_utf16char(0xFEFF, dst_is_little_endian, utf16_str);
        }
    }

    while (bytes != end)
    {
        put_utf16char(get_utf32char(bytes, end, src_is_little_endian), dst_is_little_endian, utf16_str);
    }

    return(utf16_str);
}

static std::string utf32_to_utf32(const std::string & src_utf32_str, bool src_is_little_endian, bool dst_is_little_endian)
{
    const unsigned char * bytes = reinterpret_cast<const unsigned char*>(src_utf32_str.c_str());
    const unsigned char * end = bytes + src_utf32_str.size();

    std::string dst_utf32_str;

    /*
     * if there is a BOM-(Byte Order Mark), ignore it
     */
    if (src_utf32_str.size() >= 4)
    {
        if (0xFF == bytes[0] && 0xFE == bytes[1] && 0x00 == bytes[2] && 0x00 == bytes[3])
        {
            bytes += 4;
            src_is_little_endian = true;
            put_utf32char(0xFEFF, dst_is_little_endian, dst_utf32_str);
        }
        else if (0x00 == bytes[0] && 0x00 == bytes[1] && 0xFE == bytes[2] && 0xFF == bytes[3])
        {
            bytes += 4;
            src_is_little_endian = false;
            put_utf32char(0xFEFF, dst_is_little_endian, dst_utf32_str);
        }
    }

    while (bytes != end)
    {
        put_utf32char(get_utf32char(bytes, end, src_is_little_endian), dst_is_little_endian, dst_utf32_str);
    }

    return(dst_utf32_str);
}

class CharacterSetImpl
{
public:
    virtual ~CharacterSetImpl() { }
    virtual CharacterSetImpl * clone() const = 0;
    virtual std::string str() const = 0;
    virtual std::string utfx(UTFX::TYPE utfx_type, ENDIAN::TYPE endian_type) const = 0;
    virtual std::wstring wstr() const = 0;
};

class CharacterSetString : public CharacterSetImpl
{
public:
    CharacterSetString(const std::string & ansi_str)
        : m_data(ansi_str)
        , m_is_utfx(false)
        , m_host_is_little_endian(host_is_little_endian())
    {

    }

    CharacterSetString(const std::string & utfx_str, UTFX::TYPE utfx_type, ENDIAN::TYPE endian_type)
        : m_data(utfx_str)
        , m_utfx_type(utfx_type)
        , m_is_utfx(true)
        , m_src_is_little_endian(is_little_endian(endian_type))
        , m_host_is_little_endian(host_is_little_endian())
    {

    }

private:
    virtual CharacterSetImpl * clone() const
    {
        return(new CharacterSetString(*this));
    }

    virtual std::string str() const
    {
        if (m_is_utfx)
        {
            std::string utfx_str;

#ifdef _MSC_VER
            switch (m_utfx_type)
            {
                case UTFX::UTF8:
                {
                    utfx_str = utf8_to_utf16(m_data, m_host_is_little_endian);
                    put_utf16char(0, m_host_is_little_endian, utfx_str);
                    break;
                }
                case UTFX::UTF16:
                {
                    utfx_str = utf16_to_utf16(m_data, m_src_is_little_endian, m_host_is_little_endian);
                    put_utf16char(0, m_host_is_little_endian, utfx_str);
                    break;
                }
                case UTFX::UTF32:
                {
                    utfx_str = utf32_to_utf16(m_data, m_src_is_little_endian, m_host_is_little_endian);
                    put_utf16char(0, m_host_is_little_endian, utfx_str);
                    break;
                }
                default:
                {
                    break;
                }
            }
#else
            switch (m_utfx_type)
            {
                case UTFX::UTF8:
                {
                    utfx_str = utf8_to_utf32(m_data, m_host_is_little_endian);
                    put_utf32char(0, m_host_is_little_endian, utfx_str);
                    break;
                }
                case UTFX::UTF16:
                {
                    utfx_str = utf16_to_utf32(m_data, m_src_is_little_endian, m_host_is_little_endian);
                    put_utf32char(0, m_host_is_little_endian, utfx_str);
                    break;
                }
                case UTFX::UTF32:
                {
                    utfx_str = utf32_to_utf32(m_data, m_src_is_little_endian, m_host_is_little_endian);
                    put_utf32char(0, m_host_is_little_endian, utfx_str);
                    break;
                }
                default:
                {
                    break;
                }
            }
#endif // _MSC_VER

            std::string ansi_str;
            unicode_to_ansi(reinterpret_cast<const wchar_t *>(utfx_str.c_str()), ansi_str);
            return(ansi_str);
        }
        else
        {
            return(m_data);
        }
    }

    virtual std::string utfx(UTFX::TYPE utfx_type, ENDIAN::TYPE endian_type) const
    {
        bool dst_is_little_endian = is_little_endian(endian_type);

        if (m_is_utfx)
        {
            switch (m_utfx_type)
            {
                case UTFX::UTF8:
                {
                    switch (utfx_type)
                    {
                        case UTFX::UTF8:
                        {
                            return(utf8_to_utf8(m_data));
                        }
                        case UTFX::UTF16:
                        {
                            return(utf8_to_utf16(m_data, dst_is_little_endian));
                        }
                        case UTFX::UTF32:
                        {
                            return(utf8_to_utf32(m_data, dst_is_little_endian));
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case UTFX::UTF16:
                {
                    switch (utfx_type)
                    {
                        case UTFX::UTF8:
                        {
                            return(utf16_to_utf8(m_data, m_src_is_little_endian));
                        }
                        case UTFX::UTF16:
                        {
                            return(utf16_to_utf16(m_data, m_src_is_little_endian, dst_is_little_endian));
                        }
                        case UTFX::UTF32:
                        {
                            return(utf16_to_utf32(m_data, m_src_is_little_endian, dst_is_little_endian));
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case UTFX::UTF32:
                {
                    switch (utfx_type)
                    {
                        case UTFX::UTF8:
                        {
                            return(utf32_to_utf8(m_data, m_src_is_little_endian));
                        }
                        case UTFX::UTF16:
                        {
                            return(utf32_to_utf16(m_data, m_src_is_little_endian, dst_is_little_endian));
                        }
                        case UTFX::UTF32:
                        {
                            return(utf32_to_utf32(m_data, m_src_is_little_endian, dst_is_little_endian));
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            std::string utfx_str;
            if (ansi_to_utfx(m_data.c_str(), utfx_str))
            {

#ifdef _MSC_VER
                switch (utfx_type)
                {
                    case UTFX::UTF8:
                    {
                        return(utf16_to_utf8(utfx_str, m_host_is_little_endian));
                    }
                    case UTFX::UTF16:
                    {
                        return(utf16_to_utf16(utfx_str, m_host_is_little_endian, dst_is_little_endian));
                    }
                    case UTFX::UTF32:
                    {
                        return(utf16_to_utf32(utfx_str, m_host_is_little_endian, dst_is_little_endian));
                    }
                    default:
                    {
                        break;
                    }
                }
#else
                switch (utfx_type)
                {
                    case UTFX::UTF8:
                    {
                        return(utf32_to_utf8(utfx_str, m_host_is_little_endian));
                    }
                    case UTFX::UTF16:
                    {
                        return(utf32_to_utf16(utfx_str, m_host_is_little_endian, dst_is_little_endian));
                    }
                    case UTFX::UTF32:
                    {
                        return(utf32_to_utf32(utfx_str, m_host_is_little_endian, dst_is_little_endian));
                    }
                    default:
                    {
                        break;
                    }
                }
#endif // _MSC_VER

            }
        }
        return(std::string());
    }

    virtual std::wstring wstr() const
    {
        if (m_is_utfx)
        {
            std::string utfx_str;

#ifdef _MSC_VER
            switch (m_utfx_type)
            {
                case UTFX::UTF8:
                {
                    utfx_str = utf8_to_utf16(m_data, m_host_is_little_endian);
                    break;
                }
                case UTFX::UTF16:
                {
                    utfx_str = utf16_to_utf16(m_data, m_src_is_little_endian, m_host_is_little_endian);
                    break;
                }
                case UTFX::UTF32:
                {
                    utfx_str = utf32_to_utf16(m_data, m_src_is_little_endian, m_host_is_little_endian);
                    break;
                }
                default:
                {
                    break;
                }
            }
#else
            switch (m_utfx_type)
            {
                case UTFX::UTF8:
                {
                    utfx_str = utf8_to_utf32(m_data, m_host_is_little_endian);
                    break;
                }
                case UTFX::UTF16:
                {
                    utfx_str = utf16_to_utf32(m_data, m_src_is_little_endian, m_host_is_little_endian);
                    break;
                }
                case UTFX::UTF32:
                {
                    utfx_str = utf32_to_utf32(m_data, m_src_is_little_endian, m_host_is_little_endian);
                    break;
                }
                default:
                {
                    break;
                }
            }
#endif // _MSC_VER

            return(std::wstring(reinterpret_cast<const wchar_t *>(utfx_str.c_str()), utfx_str.size() / sizeof(wchar_t)));
        }
        else
        {
            std::wstring unicode_str;
            ansi_to_unicode(m_data.c_str(), unicode_str);
            return(unicode_str);
        }
    }

private:
    std::string     m_data;
    UTFX::TYPE      m_utfx_type;
    bool            m_is_utfx;
    bool            m_src_is_little_endian;
    bool            m_host_is_little_endian;
};

class CharacterSetWString : public CharacterSetImpl
{
public:
    CharacterSetWString(const std::wstring & unicode_str)
        : m_data(unicode_str)
        , m_host_is_little_endian(host_is_little_endian())
    {

    }

    virtual CharacterSetImpl * clone() const
    {
        return(new CharacterSetWString(*this));
    }

    virtual std::string str() const
    {
        std::string ansi_str;
        unicode_to_ansi(m_data.c_str(), ansi_str);
        return(ansi_str);
    }

    virtual std::string utfx(UTFX::TYPE utfx_type, ENDIAN::TYPE endian_type) const
    {
        bool dst_is_little_endian = is_little_endian(endian_type);

#ifdef _MSC_VER
        switch (utfx_type)
        {
            case UTFX::UTF8:
            {
                return(utf16_to_utf8(std::string(reinterpret_cast<const char *>(m_data.c_str()), sizeof(wchar_t) * m_data.size()), m_host_is_little_endian));
            }
            case UTFX::UTF16:
            {
                return(utf16_to_utf16(std::string(reinterpret_cast<const char *>(m_data.c_str()), sizeof(wchar_t) * m_data.size()), m_host_is_little_endian, dst_is_little_endian));
            }
            case UTFX::UTF32:
            {
                return(utf16_to_utf32(std::string(reinterpret_cast<const char *>(m_data.c_str()), sizeof(wchar_t) * m_data.size()), m_host_is_little_endian, dst_is_little_endian));
            }
            default:
            {
                break;
            }
        }
#else
        switch (utfx_type)
        {
            case UTFX::UTF8:
            {
                return(utf32_to_utf8(std::string(reinterpret_cast<const char *>(m_data.c_str()), sizeof(wchar_t) * m_data.size()), m_host_is_little_endian));
            }
            case UTFX::UTF16:
            {
                return(utf32_to_utf16(std::string(reinterpret_cast<const char *>(m_data.c_str()), sizeof(wchar_t) * m_data.size()), m_host_is_little_endian, dst_is_little_endian));
            }
            case UTFX::UTF32:
            {
                return(utf32_to_utf32(std::string(reinterpret_cast<const char *>(m_data.c_str()), sizeof(wchar_t) * m_data.size()), m_host_is_little_endian, dst_is_little_endian));
            }
            default:
            {
                break;
            }
        }
#endif // _MSC_VER

        return(std::string());
    }

    virtual std::wstring wstr() const
    {
        return(m_data);
    }

private:
    std::wstring    m_data;
    bool            m_host_is_little_endian;
};

CharacterSet::CharacterSet(const CharacterSet & rhs)
    : m_impl(nullptr != rhs.m_impl ? rhs.m_impl->clone() : nullptr)
{

}

CharacterSet & CharacterSet::operator = (const CharacterSet & rhs)
{
    CharacterSet tmp(rhs);
    std::swap(m_impl, tmp.m_impl);
    return(*this);
}

CharacterSet::CharacterSet(const std::string & ansi_str)
    : m_impl(new CharacterSetString(ansi_str))
{

}

CharacterSet::CharacterSet(const std::string & utfx_str, UTFX::TYPE utfx_type, ENDIAN::TYPE endian_type)
    : m_impl(new CharacterSetString(utfx_str, utfx_type, endian_type))
{

}

CharacterSet::CharacterSet(const std::wstring & unicode_str)
    : m_impl(new CharacterSetWString(unicode_str))
{

}

CharacterSet::~CharacterSet()
{
    delete m_impl;
}

CharacterSet::operator std::string() const
{
    return(str());
}

CharacterSet::operator std::wstring() const
{
    return(wstr());
}

std::string CharacterSet::str() const
{
    if (nullptr == m_impl)
    {
        return(std::string());
    }
    return(m_impl->str());
}

std::wstring CharacterSet::wstr() const
{
    if (nullptr == m_impl)
    {
        return(std::wstring());
    }
    return(m_impl->wstr());
}

std::string CharacterSet::utfx(UTFX::TYPE utfx_type, ENDIAN::TYPE endian_type) const
{
    if (nullptr == m_impl)
    {
        return(std::string());
    }
    return(m_impl->utfx(utfx_type, endian_type));
}

std::string CharacterSet::utf8(ENDIAN::TYPE endian_type)
{
    return(utfx(UTFX::UTF8, endian_type));
}

std::string CharacterSet::utf16(ENDIAN::TYPE endian_type)
{
    return(utfx(UTFX::UTF16, endian_type));
}

std::string CharacterSet::utf32(ENDIAN::TYPE endian_type)
{
    return(utfx(UTFX::UTF32, endian_type));
}

std::string ansi_to_utf8(const std::string & str_ansi)
{
    return(CharacterSet(str_ansi).utf8());
}

std::wstring ansi_to_unicode(const std::string & str_ansi)
{
    return(CharacterSet(str_ansi).wstr());
}

std::string utf8_to_ansi(const std::string & str_utf8)
{
    return(CharacterSet(str_utf8, UTFX::UTF8).str());
}

std::wstring utf8_to_unicode(const std::string & str_utf8)
{
    return(CharacterSet(str_utf8, UTFX::UTF8).wstr());
}

std::string unicode_to_ansi(const std::wstring & str_unicode)
{
    return(CharacterSet(str_unicode).str());
}

std::string unicode_to_utf8(const std::wstring & str_unicode)
{
    return(CharacterSet(str_unicode).utf8());
}

NAMESPACE_STUPID_BASE_END
