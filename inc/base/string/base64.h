/********************************************************
 * Description : encode and decode of base64
 * Data        : 2013-05-19 10:20:25
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_BASE64_H
#define STUPID_BASE_BASE64_H


#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

#define BASE64_ENCODE_SIZE(in_len) (((in_len) + 2) / 3 * 4 + 1)
#define BASE64_DECODE_SIZE(in_len) (((in_len) + 3) / 4 * 3 + 1)

STUPID_CXX_API(bool) base64_encode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len);

STUPID_CXX_API(bool) base64_decode(const char * src, unsigned char * dst, size_t dst_len);

STUPID_CXX_API(bool) base64_encode(const void * src, size_t src_len, char * dst, size_t dst_len);

STUPID_CXX_API(bool) base64_decode(const char * src, void * dst, size_t dst_len);

STUPID_CXX_API(bool) base64_encode(const char * src, char * dst, size_t dst_len);

STUPID_CXX_API(bool) base64_decode(const char * src, char * dst, size_t dst_len);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_BASE64_H
