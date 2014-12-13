/********************************************************
 * Description : byte stream operator classes
 * Data        : 2013-05-19 14:04:57
 * Author      : yanrk
 * Email       : yanrkchina@hotmail.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#include <cassert>
#include <cstring>
#include "base/stream/bytestream.h"

NAMESPACE_STUPID_BASE_BEGIN

ByteStream::ByteStream(size_t len)
    : m_length(len), m_offset(0)
{

}

ByteStream::~ByteStream()
{

}

void ByteStream::host_to_net(void * obj, size_t size)
{
    assert(nullptr != obj);

    static union
    {
        unsigned short us;
        unsigned char  uc[sizeof(unsigned short)];
    } un;
    un.us = 0x0001;

    if (0x01 == un.uc[0])
    {
        unsigned char * bytes = static_cast<unsigned char *>(obj);
        for (size_t i = 0; i < size / 2; ++i)
        {
            unsigned char temp = bytes[i];
            bytes[i] = bytes[size - 1 - i];
            bytes[size - 1 - i] = temp;
        }
    }
}

void ByteStream::net_to_host(void * obj, size_t size)
{
    assert(nullptr != obj);

    host_to_net(obj, size);
}

bool ByteStream::seek(size_t offset)
{
    if (offset <= m_length)
    {
        m_offset = offset;
        return(true);
    }
    else
    {
        return(false);
    }
}

size_t ByteStream::tell()
{
    return(m_offset);
}

size_t ByteStream::left()
{
    return(m_length - m_offset);
}

void ByteStream::reset(size_t len)
{
    m_length = len;
    m_offset = 0;
}

OBStream::OBStream(unsigned char * byte, size_t len)
    : ByteStream(len), m_byte(byte)
{
    assert(nullptr != m_byte);
}

OBStream::~OBStream()
{

}

void OBStream::reset(unsigned char * byte, size_t len)
{
    ByteStream::reset(len);

    m_byte = byte;

    assert(nullptr != m_byte);
}

void OBStream::write_bytes(size_t offset, const void * obj, size_t size)
{
    assert(nullptr != obj);

    assert(offset + size <= m_length);

    memcpy(m_byte + offset, obj, size);
}

OBStream & OBStream::write(const void * obj, size_t size)
{
    assert(nullptr != obj);

    write_bytes(m_offset, obj, size);
    m_offset += size;

    return(*this);
}

void OBStream::check_write(size_t size)
{
    assert(left() >= size);
}

void OBStream::do_write_integer(uint64_t value, size_t count, unsigned char part_high)
{
    assert(0 != count && 8 >= count);

    check_write(count);

    for (size_t index = count; index > 0; --index)
    {
        m_byte[m_offset + count - index] = ((value >> ((index - 1) << 3)) & 0xff);
    }
    m_byte[m_offset] |= part_high;

    m_offset += count;
}

void OBStream::write_integer(uint64_t value)
{
    static const uint64_t BOUND_OF_1_BYTES = 0x000000000000007f;
    static const uint64_t BOUND_OF_2_BYTES = 0x0000000000003fff;
    static const uint64_t BOUND_OF_3_BYTES = 0x00000000001fffff;
    static const uint64_t BOUND_OF_4_BYTES = 0x000000000fffffff;
    static const uint64_t BOUND_OF_5_BYTES = 0x00000007ffffffff;
    static const uint64_t BOUND_OF_6_BYTES = 0x000003ffffffffff;
    static const uint64_t BOUND_OF_7_BYTES = 0x0001ffffffffffff;
    static const uint64_t BOUND_OF_8_BYTES = 0x00ffffffffffffff;

    if (value <= BOUND_OF_1_BYTES)
    {
        do_write_integer(value, 1, 0);
        return;
    }

    if (value <= BOUND_OF_2_BYTES)
    {
        do_write_integer(value, 2, 0x80);
        return;
    }

    if (value <= BOUND_OF_3_BYTES)
    {
        do_write_integer(value, 3, 0xc0);
        return;
    }

    if (value <= BOUND_OF_4_BYTES)
    {
        do_write_integer(value, 4, 0xe0);
        return;
    }

    if (value <= BOUND_OF_5_BYTES)
    {
        do_write_integer(value, 5, 0xf0);
        return;
    }

    if (value <= BOUND_OF_6_BYTES)
    {
        do_write_integer(value, 6, 0xf8);
        return;
    }

    if (value <= BOUND_OF_7_BYTES)
    {
        do_write_integer(value, 7, 0xfc);
        return;
    }

    if (value <= BOUND_OF_8_BYTES)
    {
        do_write_integer(value, 8, 0xfe);
        return;
    }

    check_write(9);
    m_byte[m_offset++] = 0xff;
    do_write_integer(value, 8, 0);
}

OBStream & OBStream::operator << (bool value)
{
    uint64_t real_value = (value ? 1 : 0);
    write_integer(real_value);
    return(*this);
}

OBStream & OBStream::operator << (int8_t value)
{
    uint64_t real_value = value & 0x00000000000000ff;
    write_integer(real_value);
    return(*this);
}

OBStream & OBStream::operator << (uint8_t value)
{
    uint64_t real_value = value & 0x00000000000000ff;
    write_integer(real_value);
    return(*this);
}

OBStream & OBStream::operator << (int16_t value)
{
    uint64_t real_value = value & 0x000000000000ffff;
    write_integer(real_value);
    return(*this);
}

OBStream & OBStream::operator << (uint16_t value)
{
    uint64_t real_value = value & 0x000000000000ffff;
    write_integer(real_value);
    return(*this);
}

OBStream & OBStream::operator << (int32_t value)
{
    uint64_t real_value = value & 0x00000000ffffffff;
    write_integer(real_value);
    return(*this);
}

OBStream & OBStream::operator << (uint32_t value)
{
    uint64_t real_value = value & 0x00000000ffffffff;
    write_integer(real_value);
    return(*this);
}

OBStream & OBStream::operator << (int64_t value)
{
    uint64_t real_value = value;
    write_integer(real_value);
    return(*this);
}

OBStream & OBStream::operator << (uint64_t value)
{
    write_integer(value);
    return(*this);
}

OBStream & OBStream::operator << (float value)
{
    assert(4 == sizeof(float));

    check_write(sizeof(float));

    // we need not consider byte-order
    // normal compile support IEEE standard
    // which parse float and double as a array
    // so "host_to_net(&value, sizeof(float))" may be wrong

    write(&value, sizeof(float));

    return(*this);
}

OBStream & OBStream::operator << (double value)
{
    assert(8 == sizeof(double));

    check_write(sizeof(double));

    // we need not consider byte-order
    // normal compile support IEEE standard
    // which parse float and double as a array
    // so "host_to_net(&value, sizeof(double))" may be wrong

    write(&value, sizeof(double));

    return(*this);
}

OBStream & OBStream::operator << (const std::string & value)
{
    uint32_t size = static_cast<uint32_t>(value.size());
    operator << (size);
    write(value.c_str(), size);
    return(*this);
}


IBStream::IBStream(const unsigned char * byte, size_t len)
    : ByteStream(len), m_byte(byte)
{
    assert(nullptr != m_byte);
}

IBStream::~IBStream()
{

}

void IBStream::reset(const unsigned char * byte, size_t len)
{
    ByteStream::reset(len);

    m_byte = byte;

    assert(nullptr != m_byte);
}

void IBStream::read_bytes(size_t offset, void * obj, size_t size)
{
    assert(nullptr != obj);

    assert(offset + size <= m_length);

    memcpy(obj, m_byte + offset, size);
}

IBStream & IBStream::read(void * obj, size_t size)
{
    assert(nullptr != obj);

    read_bytes(m_offset, obj, size);
    m_offset += size;

    return(*this);
}

void IBStream::check_read(size_t size)
{
    assert(left() >= size);
}

uint64_t IBStream::do_read_integer(unsigned char high, size_t left_count)
{
    assert(0 != left_count && 8 >= left_count);

    uint64_t value = high;

    check_read(left_count);

    for (size_t index = 0; index < left_count; ++index)
    {
        value <<= 8;
        value |= m_byte[m_offset++];
    }

    return(value);
}

uint64_t IBStream::read_integer()
{
    static const unsigned char BOUND_OF_1_BYTES = 0x7f;
    static const unsigned char BOUND_OF_2_BYTES = 0xbf;
    static const unsigned char BOUND_OF_3_BYTES = 0xdf;
    static const unsigned char BOUND_OF_4_BYTES = 0xef;
    static const unsigned char BOUND_OF_5_BYTES = 0xf7;
    static const unsigned char BOUND_OF_6_BYTES = 0xfb;
    static const unsigned char BOUND_OF_7_BYTES = 0xfd;
    static const unsigned char BOUND_OF_8_BYTES = 0xfe;

    check_read(1);

    unsigned char value = m_byte[m_offset++];

    if (value <= BOUND_OF_1_BYTES)
    {
        return(value);
    }

    if (value <= BOUND_OF_2_BYTES)
    {
        return(do_read_integer(value & ~0x80, 1));
    }

    if (value <= BOUND_OF_3_BYTES)
    {
        return(do_read_integer(value & ~0xc0, 2));
    }

    if (value <= BOUND_OF_4_BYTES)
    {
        return(do_read_integer(value & ~0xe0, 3));
    }

    if (value <= BOUND_OF_5_BYTES)
    {
        return(do_read_integer(value & ~0xf0, 4));
    }

    if (value <= BOUND_OF_6_BYTES)
    {
        return(do_read_integer(value & ~0xf8, 5));
    }

    if (value <= BOUND_OF_7_BYTES)
    {
        return(do_read_integer(value & ~0xfc, 6));
    }

    if (value <= BOUND_OF_8_BYTES)
    {
        return(do_read_integer(value & ~0xfe, 7));
    }

    return(do_read_integer(0, 8));
}

IBStream & IBStream::operator >> (bool & value)
{
    static const uint64_t MAX_VALUE = 0x01;

    uint64_t real_value = read_integer();
    assert(MAX_VALUE >= real_value);
    value = (1 == real_value ? true : false);

    return(*this);
}

IBStream & IBStream::operator >> (int8_t & value)
{
    static const uint64_t MAX_VALUE = 0xff;

    uint64_t real_value = read_integer();
    assert(MAX_VALUE >= real_value);
    value = static_cast<int8_t>(real_value);

    return(*this);
}

IBStream & IBStream::operator >> (uint8_t & value)
{
    static const uint64_t MAX_VALUE = 0xff;

    uint64_t real_value = read_integer();
    assert(MAX_VALUE >= real_value);
    value = static_cast<uint8_t>(real_value);

    return(*this);
}

IBStream & IBStream::operator >> (int16_t & value)
{
    static const uint64_t MAX_VALUE = 0xffff;

    uint64_t real_value = read_integer();
    assert(MAX_VALUE >= real_value);
    value = static_cast<int16_t>(real_value);

    return(*this);
}

IBStream & IBStream::operator >> (uint16_t & value)
{
    static const uint64_t MAX_VALUE = 0xffff;

    uint64_t real_value = read_integer();
    assert(MAX_VALUE >= real_value);
    value = static_cast<uint16_t>(real_value);

    return(*this);
}

IBStream & IBStream::operator >> (int32_t & value)
{
    static const uint64_t MAX_VALUE = 0xffffffff;

    uint64_t real_value = read_integer();
    assert(MAX_VALUE >= real_value);
    value = static_cast<int32_t>(real_value);

    return(*this);
}

IBStream & IBStream::operator >> (uint32_t & value)
{
    static const uint64_t MAX_VALUE = 0xffffffff;

    uint64_t real_value = read_integer();
    assert(MAX_VALUE >= real_value);
    value = static_cast<uint32_t>(real_value);

    return(*this);
}

IBStream & IBStream::operator >> (int64_t & value)
{
    uint64_t real_value = read_integer();
    value = static_cast<int64_t>(real_value);
    return(*this);
}

IBStream & IBStream::operator >> (uint64_t & value)
{
    value = read_integer();
    return(*this);
}

IBStream & IBStream::operator >> (float & value)
{
    assert(4 == sizeof(float));

    check_read(sizeof(float));

    // we need not consider byte-order
    // normal compile support IEEE standard
    // which parse float and double as a array
    // so "net_to_host(&value, sizeof(float))" may be wrong

    read(&value, sizeof(float));

    return(*this);
}

IBStream & IBStream::operator >> (double & value)
{
    assert(8 == sizeof(double));

    check_read(sizeof(double));

    // we need not consider byte-order
    // normal compile support IEEE standard
    // which parse float and double as a array
    // so "net_to_host(&value, sizeof(double))" may be wrong

    read(&value, sizeof(double));

    return(*this);
}

IBStream & IBStream::operator >> (std::string & value)
{
    uint32_t size = 0;
    operator >> (size);
    value.resize(size);
    read(&value[0], size);
    return(*this);
}

NAMESPACE_STUPID_BASE_END
