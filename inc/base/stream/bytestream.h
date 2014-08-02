/********************************************************
 * Description : byte stream operator classes
 * Data        : 2013-05-19 14:04:57
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_BYTE_STREAM_H
#define STUPID_BASE_BYTE_STREAM_H


#include <string>
#include <cstdint>
#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API ByteStream : private Uncopy
{
public:
    ByteStream(size_t len);
    ~ByteStream();

public:
    static void host_to_net(void * obj, size_t size);
    static void net_to_host(void * obj, size_t size);

    bool seek(size_t offset);
    size_t tell();
    size_t left();

protected:
    void reset(size_t len);

protected:
    size_t m_length;
    size_t m_offset;
};


class STUPID_API OBStream : public ByteStream
{
public:
    OBStream(unsigned char * byte, size_t len);
    ~OBStream();

public:
    void reset(unsigned char * byte, size_t len);

public:
    /* this function is different, it will not change inner-byte-offset */
    void write_bytes(size_t offset, const void * obj, size_t size);

public:
    OBStream & write(const void * obj, size_t size);

public:
    OBStream & operator << (bool);
    OBStream & operator << (int8_t);
    OBStream & operator << (uint8_t);
    OBStream & operator << (int16_t);
    OBStream & operator << (uint16_t);
    OBStream & operator << (int32_t);
    OBStream & operator << (uint32_t);
    OBStream & operator << (int64_t);
    OBStream & operator << (uint64_t);
    OBStream & operator << (float);
    OBStream & operator << (double);
    OBStream & operator << (const std::string &);

private:
    void check_write(size_t);
    void do_write_integer(uint64_t, size_t, unsigned char);
    void write_integer(uint64_t);

private:
    unsigned char * m_byte;
};


class STUPID_API IBStream : public ByteStream
{
public:
    IBStream(const unsigned char * byte, size_t len);
    ~IBStream();

public:
    void reset(const unsigned char * byte, size_t len);

public:
    /* this function is different, it will not change inner-byte-offset */
    void read_bytes(size_t offset, void * obj, size_t size);

public:
    IBStream & read(void * obj, size_t size);

public:
    IBStream & operator >> (bool &);
    IBStream & operator >> (int8_t &);
    IBStream & operator >> (uint8_t &);
    IBStream & operator >> (int16_t &);
    IBStream & operator >> (uint16_t &);
    IBStream & operator >> (int32_t &);
    IBStream & operator >> (uint32_t &);
    IBStream & operator >> (int64_t &);
    IBStream & operator >> (uint64_t &);
    IBStream & operator >> (float &);
    IBStream & operator >> (double &);
    IBStream & operator >> (std::string &);

private:
    void check_read(size_t);
    uint64_t do_read_integer(unsigned char, size_t);
    uint64_t read_integer();

private:
    const unsigned char * m_byte;
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_BYTE_STREAM_H
