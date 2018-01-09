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

#ifndef STUPID_BASE_CHARSET_H
#define STUPID_BASE_CHARSET_H


#include <string>
#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

struct UTFX
{
    enum TYPE { UTF8, UTF16, UTF32 };
};

struct ENDIAN
{
    enum TYPE { LITTLE, BIG, HOST_BYTE_ORDER };
};

class CharacterSetImpl;

class STUPID_API CharacterSet
{
public:
    CharacterSet(const std::string & ansi_str);
    CharacterSet(const std::string & utfx_str, UTFX::TYPE utfx_type, ENDIAN::TYPE endian_type = ENDIAN::HOST_BYTE_ORDER);
    CharacterSet(const std::wstring & unicode_str);

public:
    CharacterSet(const CharacterSet & rhs);
    CharacterSet & operator = (const CharacterSet & rhs);

public:
    ~CharacterSet();

public:
    operator std::string() const;
    operator std::wstring() const;

public:
    std::string str() const;
    std::wstring wstr() const;
    std::string utfx(UTFX::TYPE utfx_type, ENDIAN::TYPE endian_type = ENDIAN::HOST_BYTE_ORDER) const;
    std::string utf8(ENDIAN::TYPE endian_type = ENDIAN::HOST_BYTE_ORDER);
    std::string utf16(ENDIAN::TYPE endian_type = ENDIAN::HOST_BYTE_ORDER);
    std::string utf32(ENDIAN::TYPE endian_type = ENDIAN::HOST_BYTE_ORDER);

private:
    CharacterSetImpl * m_impl;
};

STUPID_CXX_API(std::string)  ansi_to_utf8   (const std::string & str_ansi);
STUPID_CXX_API(std::wstring) ansi_to_unicode(const std::string & str_ansi);
STUPID_CXX_API(std::string)  utf8_to_ansi   (const std::string & str_utf8);
STUPID_CXX_API(std::wstring) utf8_to_unicode(const std::string & str_utf8);
STUPID_CXX_API(std::string)  unicode_to_ansi(const std::wstring & str_unicode);
STUPID_CXX_API(std::string)  unicode_to_utf8(const std::wstring & str_unicode);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_CHARSET_H
