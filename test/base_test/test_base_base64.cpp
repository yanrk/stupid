#include "test_base.h"
#include "base/string/base64.h"

USING_NAMESPACE_STUPID_BASE

void test_base_base64(void)
{
    {
        char szSource[] = "taylor is a beautiful girl";
        char szEncode[BASE64_ENCODE_SIZE(sizeof(szSource) - 1)];
        char szDecode[sizeof(szSource)];

        base64_encode(szSource, szEncode, sizeof(szEncode));
        base64_decode(szEncode, szDecode, sizeof(szDecode));

        size_t src_len = strlen(szEncode);
        size_t dst_len = BASE64_DECODE_SIZE(src_len);
        char * pszDecode = new char[dst_len];

        base64_decode(szEncode, pszDecode, dst_len);

        printf("source: %s\n", szSource);
        printf("encode: %s\n", szEncode);
        printf("decode: %s\n", szDecode);
        printf("decode: %s\n\n", pszDecode);

        delete [] pszDecode;
    }

    {
        char szSource[] = "taylor is a beautiful girl1";
        char szEncode[BASE64_ENCODE_SIZE(sizeof(szSource) - 1)];
        char szDecode[sizeof(szSource)];

        base64_encode(szSource, szEncode, sizeof(szEncode));
        base64_decode(szEncode, szDecode, sizeof(szDecode));

        size_t src_len = strlen(szEncode);
        size_t dst_len = BASE64_DECODE_SIZE(src_len);
        char * pszDecode = new char[dst_len];

        base64_decode(szEncode, pszDecode, dst_len);

        printf("source: %s\n", szSource);
        printf("encode: %s\n", szEncode);
        printf("decode: %s\n", szDecode);
        printf("decode: %s\n\n", pszDecode);

        delete [] pszDecode;
    }

    {
        char szSource[] = "taylor is a beautiful girl12";
        char szEncode[BASE64_ENCODE_SIZE(sizeof(szSource) - 1)];
        char szDecode[sizeof(szSource)];

        base64_encode(szSource, szEncode, sizeof(szEncode));
        base64_decode(szEncode, szDecode, sizeof(szDecode));

        size_t src_len = strlen(szEncode);
        size_t dst_len = BASE64_DECODE_SIZE(src_len);
        char * pszDecode = new char[dst_len];

        base64_decode(szEncode, pszDecode, dst_len);

        printf("source: %s\n", szSource);
        printf("encode: %s\n", szEncode);
        printf("decode: %s\n", szDecode);
        printf("decode: %s\n\n", pszDecode);

        delete [] pszDecode;
    }

    {
        char szSource[] = "taylor is a beautiful girl123";
        char szEncode[BASE64_ENCODE_SIZE(sizeof(szSource) - 1)];
        char szDecode[sizeof(szSource)];

        base64_encode(szSource, szEncode, sizeof(szEncode));
        base64_decode(szEncode, szDecode, sizeof(szDecode));

        size_t src_len = strlen(szEncode);
        size_t dst_len = BASE64_DECODE_SIZE(src_len);
        char * pszDecode = new char[dst_len];

        base64_decode(szEncode, pszDecode, dst_len);

        printf("source: %s\n", szSource);
        printf("encode: %s\n", szEncode);
        printf("decode: %s\n", szDecode);
        printf("decode: %s\n\n", pszDecode);

        delete [] pszDecode;
    }

    {
        char szSource[] = "taylor is a beautiful girl1234";
        char szEncode[BASE64_ENCODE_SIZE(sizeof(szSource) - 1)];
        char szDecode[sizeof(szSource)];

        base64_encode(szSource, szEncode, sizeof(szEncode));
        base64_decode(szEncode, szDecode, sizeof(szDecode));

        size_t src_len = strlen(szEncode);
        size_t dst_len = BASE64_DECODE_SIZE(src_len);
        char * pszDecode = new char[dst_len];

        base64_decode(szEncode, pszDecode, dst_len);

        printf("source: %s\n", szSource);
        printf("encode: %s\n", szEncode);
        printf("decode: %s\n", szDecode);
        printf("decode: %s\n\n", pszDecode);

        delete [] pszDecode;
    }

    {
        char szSource[] = "taylor is a beautiful girl12345";
        char szEncode[BASE64_ENCODE_SIZE(sizeof(szSource) - 1)];
        char szDecode[sizeof(szSource)];

        base64_encode(szSource, szEncode, sizeof(szEncode));
        base64_decode(szEncode, szDecode, sizeof(szDecode));

        size_t src_len = strlen(szEncode);
        size_t dst_len = BASE64_DECODE_SIZE(src_len);
        char * pszDecode = new char[dst_len];

        base64_decode(szEncode, pszDecode, dst_len);

        printf("source: %s\n", szSource);
        printf("encode: %s\n", szEncode);
        printf("decode: %s\n", szDecode);
        printf("decode: %s\n\n", pszDecode);

        delete [] pszDecode;
    }
}
