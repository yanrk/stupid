/********************************************************
 * Description : conversions of binary and hexadecimal
 * Data        : 2013-05-19 11:53:27
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cstring>
#include "base/string/hex.h"

static const char hex_lower_table[] = 
{
    '0', '1', '2', '3', '4', '5', '6', '7', 
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'  
};

static const char hex_upper_table[] = 
{
    '0', '1', '2', '3', '4', '5', '6', '7', 
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'  
};

static unsigned char hex_index(const char cSymbol)
{
    if ('0' <= cSymbol && '9' >= cSymbol)
    {
        return cSymbol - '0';
    }
    else if ('a' <= cSymbol && 'f' >= cSymbol)
    {
        return cSymbol - 'a' + 10;
    }
    else if ('A' <= cSymbol && 'F' >= cSymbol)
    {
        return cSymbol - 'A' + 10;
    }
    else
    {
        return 16;
    }
}

NAMESPACE_STUPID_BASE_BEGIN

bool binary_to_hex(const unsigned char src, char dst[2], bool lower)
{
    if (nullptr == dst)
    {
        return false;
    }

    if (lower)
    {
        dst[0] = hex_lower_table[(src >> 4) & 0x0F];
        dst[1] = hex_lower_table[src & 0x0F];
    }
    else
    {
        dst[0] = hex_upper_table[(src >> 4) & 0x0F];
        dst[1] = hex_upper_table[src & 0x0F];
    }

    return true;
}

bool hex_to_binary(const char src[2], unsigned char * dst)
{
    if (nullptr == src || nullptr == dst)
    {
        return false;
    }

    unsigned char hig = hex_index(src[0]);
    unsigned char low = hex_index(src[1]);

    if (16 <= hig || 16 <= low)
    {
        return false;
    }

    dst[0] = (hig << 4) | low;

    return true;
}

bool hex_encode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len, bool lower)
{
    if (nullptr == src || nullptr == dst || HEX_ENCODE_SIZE(src_len) > dst_len)
    {
        return false;
    }

    for (size_t index = 0; index < src_len; ++index)
    {
        if (!binary_to_hex(src[0], dst, lower))
        {
            return false;
        }

        src += 1;
        dst += 2;
    }

    dst[0] = '\0';

    return true;
}

bool hex_decode(const char * src, unsigned char * dst, size_t dst_len)
{
    if (nullptr == src || nullptr == dst)
    {
        return false;
    }

    size_t src_len = strlen(src);

    if (0 != src_len % 2)
    {
        return false;
    }

    if (0 == src_len)
    {
        return true;
    }

    if (HEX_DECODE_SIZE(src_len) - 1 > dst_len)
    {
        return false;
    }

    for (size_t index = 0; index < src_len; index += 2)
    {
        if (!hex_to_binary(src, dst))
        {
            return false;
        }

        src += 2;
        dst += 1;
    }

    return true;
}

bool hex_encode(const void * src, size_t src_len, char * dst, size_t dst_len, bool lower)
{
    return hex_encode(reinterpret_cast<const unsigned char *>(src), src_len, dst, dst_len, lower);
}

bool hex_decode(const char * src, void * dst, size_t dst_len)
{
    return hex_decode(src, reinterpret_cast<unsigned char *>(dst), dst_len);
}

bool hex_encode(const char * src, char * dst, size_t dst_len, bool lower)
{
    if (nullptr == src)
    {
        return false;
    }

    size_t src_len = strlen(src);

    return hex_encode(src, src_len, dst, dst_len, lower);
}

bool hex_decode(const char * src, char * dst, size_t dst_len)
{
    if (nullptr == dst || 0 == dst_len)
    {
        return false;
    }

    memset(dst, 0x00, dst_len);

    return hex_decode(src, reinterpret_cast<unsigned char *>(dst), dst_len - 1);
}

NAMESPACE_STUPID_BASE_END
