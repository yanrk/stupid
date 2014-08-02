#include "test_base.h"
#include "base/string/bcd.h"

USING_NAMESPACE_STUPID_BASE

void test_base_bcd(void)
{
    {
        char szSource[] = "123";
        unsigned char szEncode[BCD_ENCODE_SIZE(sizeof(szSource) - 1)];
        char szDecode[sizeof(szSource)];

        size_t encode_len = 0;
        bcd_encode(szSource, szEncode, sizeof(szEncode), encode_len);
        bcd_decode(szEncode, encode_len, szDecode, sizeof(szDecode));

        if (0 != memcmp(szSource, szDecode, sizeof(szSource)))
        {
            printf("bcd encode-decode for \"%s\" legit\n", szSource);
        }
    }

    {
        char szSource[] = "1234";
        unsigned char szEncode[BCD_ENCODE_SIZE(sizeof(szSource) - 1)];
        char szDecode[sizeof(szSource)];

        size_t encode_len = 0;
        bcd_encode(szSource, szEncode, sizeof(szEncode), encode_len);
        bcd_decode(szEncode, encode_len, szDecode, sizeof(szDecode));

        if (0 != memcmp(szSource, szDecode, sizeof(szSource)))
        {
            printf("bcd encode-decode for \"%s\" legit\n", szSource);
        }
    }
}
