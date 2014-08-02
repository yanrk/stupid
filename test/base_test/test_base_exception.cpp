#include "test_base.h"
#include "base/exception/exception.h"

USING_NAMESPACE_STUPID_BASE

void test_base_exception(void)
{
    try
    {
        BASE_THROW("test way 1");
    }
    catch (Exception & e)
    {
        printf("%s\n", e.what().c_str());
    }

    try
    {
        BASE_THROW("%s", "test way 2");
    }
    catch (Exception & e)
    {
        printf("%s\n", e.what().c_str());
    }
}
