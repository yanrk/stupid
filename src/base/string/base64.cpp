/********************************************************
 * Description : encode and decode of base64
 * Data        : 2013-05-19 10:20:25
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <cstring>
#include "base/string/base64.h"

static const char base64_table[] = 
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
    'w', 'x', 'y', 'z', '0', '1', '2', '3', 
    '4', '5', '6', '7', '8', '9', '+', '/'  
};

static unsigned char base64_index(const char cSymbol)
{
    if ('A' <= cSymbol && 'Z' >= cSymbol)
    {
        return(cSymbol - 'A' + 0);
    }
    else if ('a' <= cSymbol && 'z' >= cSymbol)
    {
        return(cSymbol - 'a' + 26);
    }
    else if ('0' <= cSymbol && '9' >= cSymbol)
    {
        return(cSymbol - '0' + 52);
    }
    else if ('+' == cSymbol)
    {
        return(62);
    }
    else if ('/' == cSymbol)
    {
        return(63);
    }
    else if ('=' == cSymbol)
    {
        return(0);
    }
    else
    {
        return(64);
    }
}

NAMESPACE_STUPID_BASE_BEGIN

bool base64_encode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len)
{
    if (nullptr == src || nullptr == dst || BASE64_ENCODE_SIZE(src_len) > dst_len)
    {
        return(false);
    }

    for (size_t index = 0; index < src_len; index += 3)
    {
        unsigned char src_tmp[3] = { 0 };
        size_t copy_len = (src_len - index >= 3 ? 3 : src_len - index);
        memcpy(src_tmp, src, copy_len);

        dst[0] = base64_table[((src_tmp[0] >> 2) & 0x3F)];
        dst[1] = base64_table[((src_tmp[0] << 4) & 0x30) | ((src_tmp[1] >> 4) & 0x0F)];
        dst[2] = base64_table[((src_tmp[1] << 2) & 0x3C) | ((src_tmp[2] >> 6) & 0x03)];
        dst[3] = base64_table[((src_tmp[2]) & 0x3F)];

        src += 3;
        dst += 4;
    }

    dst[0] = '\0';

    switch (src_len % 3)
    {
        case 1:
        {
            dst[-2] = '=';
        }
        case 2:
        {
            dst[-1] = '=';
        }
    }

    return(true);
}

bool base64_decode(const char * src, unsigned char * dst, size_t dst_len)
{
    if (nullptr == src || nullptr == dst)
    {
        return(false);
    }

    size_t src_len = strlen(src);

    if (0 != src_len % 4)
    {
        return(false);
    }

    if (0 == src_len)
    {
        return(true);
    }

    for (size_t index = 0; index < src_len - 2; ++index)
    {
        if ('=' == src[index])
        {
            return(false);
        }
    }

    size_t out_len = 0;

    for (size_t index = 0; index < src_len; index += 4)
    {
        unsigned char src_tmp[4] = { 0 };
        size_t pad_cnt = 0;
        for (size_t n = 0; n < 4; ++n)
        {
            if ('=' == src[n])
            {
                ++pad_cnt;
            }

            if ((src_tmp[n] = base64_index(src[n])) > 63)
            {
                return(false);
            }
        }

        out_len += 3 - pad_cnt;

        if (out_len > dst_len)
        {
            return(false);
        }

        switch (pad_cnt)
        {
            case 0:
            {
                dst[2] = (src_tmp[2] << 6) | (src_tmp[3]);
            }
            case 1:
            {
                dst[1] = (src_tmp[1] << 4) | (src_tmp[2] >> 2);
            }
            case 2:
            {
                dst[0] = (src_tmp[0] << 2) | (src_tmp[1] >> 4);
                break;
            }
            default:
            {
                return(false);
                break;
            }
        }

        src += 4;
        dst += 3 - pad_cnt;
    }

    if ('=' == src[-2] && '=' != src[-1])
    {
        return(false);
    }

    return(true);
}

bool base64_encode(const void * src, size_t src_len, char * dst, size_t dst_len)
{
    return(base64_encode(reinterpret_cast<const unsigned char *>(src), src_len, dst, dst_len));
}

bool base64_decode(const char * src, void * dst, size_t dst_len)
{
    return(base64_decode(src, reinterpret_cast<unsigned char *>(dst), dst_len));
}

bool base64_encode(const char * src, char * dst, size_t dst_len)
{
    if (nullptr == src)
    {
        return(false);
    }

    size_t src_len = strlen(src);

    return(base64_encode(src, src_len, dst, dst_len));
}

bool base64_decode(const char * src, char * dst, size_t dst_len)
{
    if (nullptr == dst || 0 == dst_len)
    {
        return(false);
    }

    memset(dst, 0, dst_len);

    return(base64_decode(src, reinterpret_cast<unsigned char *>(dst), dst_len - 1));
}

NAMESPACE_STUPID_BASE_END
