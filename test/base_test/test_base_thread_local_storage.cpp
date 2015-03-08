#include "test_base.h"
#include "base/utility/guard.h"
#include "base/thread/thread.h"
#include "base/thread/thread_local_storage.h"
#include "base/locker/locker.h"
#include "base/time/time.h"

USING_NAMESPACE_STUPID_BASE

static ThreadLocalStorage   s_thread_local_storage;
static ThreadLocker         s_stdout_locker;

struct TEST_THREAD
{
    Thread     * thread;
    int          value;
};

static thread_return_t STUPID_STDCALL thread_run(thread_argument_t argument)
{
    TEST_THREAD * test = reinterpret_cast<TEST_THREAD *>(argument);
    if (nullptr == test || nullptr == test->thread)
    {
        return(THREAD_DEFAULT_RET);
    }

    void * thread_specific_value = nullptr;

    {
        Guard<ThreadLocker> stdout_guard(s_stdout_locker);
        printf("[%s] start\n", test->thread->thread_name().c_str());
    }

    thread_specific_value = reinterpret_cast<void *>(test->value);
    s_thread_local_storage.set_value(thread_specific_value);

    {
        Guard<ThreadLocker> stdout_guard(s_stdout_locker);
        printf("[%s] set value [%u]\n", test->thread->thread_name().c_str(), reinterpret_cast<size_t>(thread_specific_value));
    }

    while (test->thread->running())
    {
        thread_specific_value = s_thread_local_storage.get_value();

        {
            Guard<ThreadLocker> stdout_guard(s_stdout_locker);
            printf("[%s] get value [%u]\n", test->thread->thread_name().c_str(), reinterpret_cast<size_t>(thread_specific_value));
        }

        stupid_ms_sleep(1);
    }

    {
        Guard<ThreadLocker> stdout_guard(s_stdout_locker);
        printf("[%s] end\n", test->thread->thread_name().c_str());
    }

    return(THREAD_DEFAULT_RET);
}

void test_base_thread_local_storage(void)
{
    Thread thread_1;
    Thread thread_2;
    TEST_THREAD thread_argument_1 = { &thread_1, 1111 };
    TEST_THREAD thread_argument_2 = { &thread_2, 2222 };

    thread_1.set_thread_args(thread_run, &thread_argument_1, "thread 1");
    thread_2.set_thread_args(thread_run, &thread_argument_2, "thread 2");

    thread_1.acquire();
    thread_2.acquire();

    stupid_ms_sleep(100);

    thread_1.release();
    thread_2.release();
}
