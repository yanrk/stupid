#include "test_base.h"
#include "base/common/common.h"

USING_NAMESPACE_STUPID_BASE

void test_base_memory(void)
{
    {
        int * i = nullptr;
        STUPID_NEW(i, int);
        *i = 5;
        STUPID_DEL(i);
        assert(nullptr == i);
    }

    {
        int * i = nullptr;
        STUPID_NEW(i, int(1));
        assert(1 == *i);
        STUPID_DEL(i);
        assert(nullptr == i);
    }

    {
        int * i = nullptr;
        STUPID_NEW_ARR(i, int, 1);
        i[0] = 5;
        STUPID_DEL_ARR(i);
        assert(nullptr == i);
    }

    {
        std::string * s = nullptr;
        STUPID_NEW(s, std::string);
        *s = "hello world";
        STUPID_DEL(s);
        assert(nullptr == s);
    }

    {
        std::string * s = nullptr;
        STUPID_NEW(s, std::string("hello world"));
        assert("hello world" == *s);
        STUPID_DEL(s);
        assert(nullptr == s);
    }

    {
        std::string * s = nullptr;
        STUPID_NEW_ARR(s, std::string, 1);
        s[0] = "hello world";
        STUPID_DEL_ARR(s);
        assert(nullptr == s);
    }
}
