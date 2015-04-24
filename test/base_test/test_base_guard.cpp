#include "test_base.h"
#include "base/utility/guard.h"
#include "base/locker/locker.h"

USING_NAMESPACE_STUPID_BASE

void test_base_guard(void)
{
    ThreadLocker thread_locker;
    {
        Guard<ThreadLocker> guard(thread_locker);
    }

    ProcessLocker process_locker;
    {
        Guard<ProcessLocker> guard(process_locker);
    }
}
