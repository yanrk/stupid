/********************************************************
 * Description : encode and decode of bcd
 * Data        : 2014-03-22 17:03:12
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_BCD_H
#define STUPID_BASE_BCD_H


#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

#define BCD_ENCODE_SIZE(in_len) (((in_len) + 1) / 2)
#define BCD_DECODE_SIZE(in_len) (((in_len) * 2) + 1)

STUPID_CXX_API(bool) bcd_encode(const char * src, unsigned char * dst, size_t dst_siz, size_t & dst_len);

STUPID_CXX_API(bool) bcd_decode(const unsigned char * src, size_t src_len, char * dst, size_t dst_siz);

STUPID_CXX_API(bool) bcd_encode(const char * src, size_t src_len, unsigned char * dst, size_t dst_siz, size_t & dst_len);

STUPID_CXX_API(bool) bcd_decode(const unsigned char * src, size_t src_len, char * dst, size_t dst_siz, size_t & dst_len);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_BCD_H
