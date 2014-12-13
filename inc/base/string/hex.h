/********************************************************
 * Description : conversions of binary and hexadecimal
 * Data        : 2013-05-19 11:53:27
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_HEX_H
#define STUPID_BASE_HEX_H


#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

#define HEX_ENCODE_SIZE(in_len) ((in_len) * 2 + 1)
#define HEX_DECODE_SIZE(in_len) ((in_len) / 2 + 1)

STUPID_CXX_API(bool) binary_to_hex(const unsigned char src, char dst[2], bool lower = true);

STUPID_CXX_API(bool) hex_to_binary(const char src[2], unsigned char * dst);

STUPID_CXX_API(bool) hex_encode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len, bool lower = true);

STUPID_CXX_API(bool) hex_decode(const char * src, unsigned char * dst, size_t dst_len);

STUPID_CXX_API(bool) hex_encode(const void * src, size_t src_len, char * dst, size_t dst_len, bool lower = true);

STUPID_CXX_API(bool) hex_decode(const char * src, void * dst, size_t dst_len);

STUPID_CXX_API(bool) hex_encode(const char * src, char * dst, size_t dst_len, bool lower = true);

STUPID_CXX_API(bool) hex_decode(const char * src, char * dst, size_t dst_len);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_HEX_H
