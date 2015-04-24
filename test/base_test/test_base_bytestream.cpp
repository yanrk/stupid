#include "test_base.h"
#include "base/stream/bytestream.h"

USING_NAMESPACE_STUPID_BASE

static void test_bytestream_1(void)
{
    unsigned char buffer[100];

    bool in_b = true;
    int8_t in_8 = 25;
    uint8_t uin_8 = 255;
    int16_t in_16 = 3456;
    uint16_t uin_16 = 34567;
    int32_t in_32 = 12345678;
    uint32_t uin_32 = 12345678;
    int64_t in_64 = 11111111111111111;
    uint64_t uin_64 = 11111111111111111;
    float in_f = 123.4567f;
    double in_d = 123456789.123456789;
    std::string in_str = "hello world";

    OBStream obs(buffer, 100);
    obs << in_b << in_8 << uin_8 << in_16 << uin_16 
        << in_32 << uin_32 << in_64 << uin_64 
        << in_f << in_d << in_str;

    bool out_b = false;
    int8_t out_8 = 0;
    uint8_t uout_8 = 0;
    int16_t out_16 = 0;
    uint16_t uout_16 = 0;
    int32_t out_32 = 0;
    uint32_t uout_32 = 0;
    int64_t out_64 = 0;
    uint64_t uout_64 = 0;
    float out_f = 0.0f;
    double out_d = 0.0;
    std::string out_str = "";

    IBStream ibs(buffer, obs.tell());
    ibs >> out_b >> out_8 >> uout_8 >> out_16 >> uout_16 
        >> out_32 >> uout_32 >> out_64 >> uout_64 
        >> out_f >> out_d >> out_str;

    if (in_8 != out_8 || uin_8 != uout_8 || 
        in_16 != out_16 || uin_16 != uout_16 || 
        in_32 != out_32 || uin_32 != uout_32 || 
        in_64 != out_64 || uin_64 != uout_64 || 
        in_b != out_b || in_str != out_str)
    {
        assert(false);
    }

    if (in_f - out_f > 0.000001f || in_f - out_f < -0.000001f)
    {
        assert(false);
    }

    if (in_d - out_d > 0.00000000000001 || in_d - out_d < -0.00000000000001)
    {
        assert(false);
    }
}

static void test_bytestream_2(void)
{
    static const uint64_t BOUND_OF_1_BYTES = 0x000000000000007f;
    static const uint64_t BOUND_OF_2_BYTES = 0x0000000000003fff;
    static const uint64_t BOUND_OF_3_BYTES = 0x00000000001fffff;
    static const uint64_t BOUND_OF_4_BYTES = 0x000000000fffffff;
    static const uint64_t BOUND_OF_5_BYTES = 0x00000007ffffffff;
    static const uint64_t BOUND_OF_6_BYTES = 0x000003ffffffffff;
    static const uint64_t BOUND_OF_7_BYTES = 0x0001ffffffffffff;
    static const uint64_t BOUND_OF_8_BYTES = 0x00ffffffffffffff;

    {
        unsigned char buffer[100];

        uint64_t in_0 = 0x0000000000000000;
        uint64_t in_1 = 0xffffffffffffffff;

        OBStream obs(buffer, 100);
        obs << in_0 << in_1;

        uint64_t out_0 = 1;
        uint64_t out_1 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_0 >> out_1;

        if (in_0 != out_0 || in_1 != out_1 || 0 != ibs.left())
        {
            assert(false);
        }
    }

    {
        unsigned char buffer[100];

        uint64_t in_1 = BOUND_OF_1_BYTES - 1;
        uint64_t in_2 = BOUND_OF_1_BYTES;
        uint64_t in_3 = BOUND_OF_1_BYTES + 1;

        OBStream obs(buffer, 100);
        obs << in_1 << in_2 << in_3;

        uint64_t out_1 = 1;
        uint64_t out_2 = 1;
        uint64_t out_3 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_1 >> out_2 >> out_3;

        if (in_1 != out_1 || in_2 != out_2 || in_3 != out_3 || 0 != ibs.left())
        {
            assert(false);
        }
    }

    {
        unsigned char buffer[100];

        uint64_t in_1 = BOUND_OF_2_BYTES - 1;
        uint64_t in_2 = BOUND_OF_2_BYTES;
        uint64_t in_3 = BOUND_OF_2_BYTES + 1;

        OBStream obs(buffer, 100);
        obs << in_1 << in_2 << in_3;

        uint64_t out_1 = 1;
        uint64_t out_2 = 1;
        uint64_t out_3 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_1 >> out_2 >> out_3;

        if (in_1 != out_1 || in_2 != out_2 || in_3 != out_3 || 0 != ibs.left())
        {
            assert(false);
        }
    }

    {
        unsigned char buffer[100];

        uint64_t in_1 = BOUND_OF_3_BYTES - 1;
        uint64_t in_2 = BOUND_OF_3_BYTES;
        uint64_t in_3 = BOUND_OF_3_BYTES + 1;

        OBStream obs(buffer, 100);
        obs << in_1 << in_2 << in_3;

        uint64_t out_1 = 1;
        uint64_t out_2 = 1;
        uint64_t out_3 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_1 >> out_2 >> out_3;

        if (in_1 != out_1 || in_2 != out_2 || in_3 != out_3 || 0 != ibs.left())
        {
            assert(false);
        }
    }

    {
        unsigned char buffer[100];

        uint64_t in_1 = BOUND_OF_4_BYTES - 1;
        uint64_t in_2 = BOUND_OF_4_BYTES;
        uint64_t in_3 = BOUND_OF_4_BYTES + 1;

        OBStream obs(buffer, 100);
        obs << in_1 << in_2 << in_3;

        uint64_t out_1 = 1;
        uint64_t out_2 = 1;
        uint64_t out_3 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_1 >> out_2 >> out_3;

        if (in_1 != out_1 || in_2 != out_2 || in_3 != out_3 || 0 != ibs.left())
        {
            assert(false);
        }
    }

    {
        unsigned char buffer[100];

        uint64_t in_1 = BOUND_OF_5_BYTES - 1;
        uint64_t in_2 = BOUND_OF_5_BYTES;
        uint64_t in_3 = BOUND_OF_5_BYTES + 1;

        OBStream obs(buffer, 100);
        obs << in_1 << in_2 << in_3;

        uint64_t out_1 = 1;
        uint64_t out_2 = 1;
        uint64_t out_3 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_1 >> out_2 >> out_3;

        if (in_1 != out_1 || in_2 != out_2 || in_3 != out_3 || 0 != ibs.left())
        {
            assert(false);
        }
    }

    {
        unsigned char buffer[100];

        uint64_t in_1 = BOUND_OF_6_BYTES - 1;
        uint64_t in_2 = BOUND_OF_6_BYTES;
        uint64_t in_3 = BOUND_OF_6_BYTES + 1;

        OBStream obs(buffer, 100);
        obs << in_1 << in_2 << in_3;

        uint64_t out_1 = 1;
        uint64_t out_2 = 1;
        uint64_t out_3 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_1 >> out_2 >> out_3;

        if (in_1 != out_1 || in_2 != out_2 || in_3 != out_3 || 0 != ibs.left())
        {
            assert(false);
        }
    }

    {
        unsigned char buffer[100];

        uint64_t in_1 = BOUND_OF_7_BYTES - 1;
        uint64_t in_2 = BOUND_OF_7_BYTES;
        uint64_t in_3 = BOUND_OF_7_BYTES + 1;

        OBStream obs(buffer, 100);
        obs << in_1 << in_2 << in_3;

        uint64_t out_1 = 1;
        uint64_t out_2 = 1;
        uint64_t out_3 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_1 >> out_2 >> out_3;

        if (in_1 != out_1 || in_2 != out_2 || in_3 != out_3 || 0 != ibs.left())
        {
            assert(false);
        }
    }

    {
        unsigned char buffer[100];

        uint64_t in_1 = BOUND_OF_8_BYTES - 1;
        uint64_t in_2 = BOUND_OF_8_BYTES;
        uint64_t in_3 = BOUND_OF_8_BYTES + 1;

        OBStream obs(buffer, 100);
        obs << in_1 << in_2 << in_3;

        uint64_t out_1 = 1;
        uint64_t out_2 = 1;
        uint64_t out_3 = 1;

        IBStream ibs(buffer, obs.tell());
        ibs >> out_1 >> out_2 >> out_3;

        if (in_1 != out_1 || in_2 != out_2 || in_3 != out_3 || 0 != ibs.left())
        {
            assert(false);
        }
    }
}

void test_base_bytestream(void)
{
    test_bytestream_1();
    test_bytestream_2();
}
