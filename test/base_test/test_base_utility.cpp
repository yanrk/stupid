#include "test_base.h"
#include "base/utility/utility.h"

USING_NAMESPACE_STUPID_BASE

void test_base_utility(void)
{
    printf("pid: %u, tid: %u\n", get_pid(), get_tid());
}
