#include "test_base.h"
#include "base/string/hex.h"

USING_NAMESPACE_STUPID_BASE

void test_base_hex(void)
{
    for (int i = 0; i < 256; ++i)
    {
        unsigned char src = static_cast<unsigned char>(i);
        char enc[3] = { 0 };
        unsigned char dec = '\0';

        bool benc = binary_to_hex(src, enc);
        if (!benc)
        {
            assert(false);
            return;
        }

        bool bdec = hex_to_binary(enc, &dec);
        if (!bdec)
        {
            assert(false);
            return;
        }

        if (src != dec)
        {
            printf("3 badly: %d - %d - %s\n", src, dec, enc);
            assert(false);
            return;
        }

        printf("%3d - %s\n", i, enc);
    }

    {
        const char src[] = "Hello World";
        char enc[HEX_ENCODE_SIZE(sizeof(src) / sizeof(src[0]))];
        char dec[sizeof(src)];

        if (!hex_encode(src, enc, sizeof(enc)))
        {
            assert(false);
            return;
        }

        if (!hex_decode(enc, dec, sizeof(dec)))
        {
            assert(false);
            return;
        }

        int enc_len = strlen(enc);
        int dst_len = HEX_DECODE_SIZE(enc_len);
        char * dst = new char[dst_len];

        if (!hex_decode(enc, dst, dst_len))
        {
            assert(false);
            return;
        }

        printf("src: %s\n", src);
        printf("enc: %s\n", enc);
        printf("dec: %s\n", dec);
        printf("dst: %s\n", dst);

        delete [] dst;
    }
}
