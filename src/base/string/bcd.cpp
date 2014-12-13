/********************************************************
 * Description : encode and decode of bcd
 * Data        : 2014-03-22 17:03:12
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <cstring>
#include <algorithm>
#include "base/string/bcd.h"

struct legit_bcd_encode_element
{
    bool operator() (char ele) const
    {
        return(ele >= '0' && ele <= '9');
    }
};

struct legit_bcd_decode_element
{
    bool operator() (unsigned char ele) const
    {
        unsigned char hig = (ele & 0xF0);
        unsigned char low = (ele & 0x0F);
        return(hig <= 0x90 && low <= 0x09);
    }
};

NAMESPACE_STUPID_BASE_BEGIN

bool bcd_encode(const char * src, size_t src_len, unsigned char * dst, size_t dst_siz, size_t & dst_len)
{
    dst_len = 0;

    if (nullptr == src || nullptr == dst || BCD_ENCODE_SIZE(src_len) > dst_siz)
    {
        return(false);
    }

    if (0 == src_len)
    {
        return(true);
    }

    if (src + src_len != std::find_if_not(src, src + src_len, legit_bcd_encode_element()))
    {
        return(false);
    }

    if (1 == src_len % 2)
    {
        dst[0] = static_cast<unsigned char>(src[0] - '0');
        src += 1;
        src_len -= 1;
        dst += 1;
        dst_len += 1;
    }

    while (0 != src_len)
    {
        dst[0] = static_cast<unsigned char>(((src[0] - '0') << 4) | ((src[1] - '0')));
        src += 2;
        src_len -= 2;
        dst += 1;
        dst_len += 1;
    }

    return(true);
}

bool bcd_encode(const char * src, unsigned char * dst, size_t dst_siz, size_t & dst_len)
{
    if (nullptr == src)
    {
        dst_len = 0;
        return(false);
    }
    return(bcd_encode(src, strlen(src), dst, dst_siz, dst_len));
}

bool bcd_decode(const unsigned char * src, size_t src_len, char * dst, size_t dst_siz, size_t & dst_len)
{
    dst_len = 0;

    if (nullptr == src || nullptr == dst || 0 > dst_siz)
    {
        return(false);
    }

    if ((BCD_DECODE_SIZE(src_len) - 1 > dst_siz) || (src[0] >= 0x10 && BCD_DECODE_SIZE(src_len) > dst_siz))
    {
        return(false);
    }

    if (0 == src_len)
    {
        dst[0] = '\0';
        return(true);
    }

    if (src + src_len != std::find_if_not(src, src + src_len, legit_bcd_decode_element()))
    {
        return(false);
    }

    if (src[0] < 0x10)
    {
        dst[0] = static_cast<char>(src[0] + '0');
        src += 1;
        src_len -= 1;
        dst += 1;
        dst_len += 1;
    }

    while (0 != src_len)
    {
        dst[0] = static_cast<char>(((src[0] >> 4) & 0x0F) + '0');
        dst[1] = static_cast<char>(((src[0] & 0x0F)) + '0');
        src += 1;
        src_len -= 1;
        dst += 2;
        dst_len += 2;
    }

    dst[0] = '\0';

    return(true);
}

bool bcd_decode(const unsigned char * src, size_t src_len, char * dst, size_t dst_siz)
{
    size_t dst_len = 0;
    return(bcd_decode(src, src_len, dst, dst_siz, dst_len));
}

NAMESPACE_STUPID_BASE_END
