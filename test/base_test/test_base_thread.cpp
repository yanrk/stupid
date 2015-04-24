#include "test_base.h"
#include "base/utility/guard.h"
#include "base/thread/thread.h"
#include "base/locker/locker.h"
#include "base/time/time.h"

USING_NAMESPACE_STUPID_BASE

static Thread         s_thread_1;
static Thread         s_thread_2;
static ThreadLocker   s_stdout_locker;

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

    s_stdout_locker.acquire();
    printf("[%s] start\n", test->thread->thread_name().c_str());
    s_stdout_locker.release();

    while (test->thread->running())
    {
        stupid_ms_sleep(1);
        Guard<ThreadLocker> stdout_guard(s_stdout_locker);
        printf("[%s] %d\n", test->thread->thread_name().c_str(), test->value);
    }

    s_stdout_locker.acquire();
    printf("[%s] end\n", test->thread->thread_name().c_str());
    s_stdout_locker.release();

    return(THREAD_DEFAULT_RET);
}

static thread_return_t STUPID_STDCALL detached_thread_run(thread_argument_t argument)
{
    argument = nullptr;

    s_stdout_locker.acquire();
    printf("[detached thread] start\n");
    s_stdout_locker.release();

    for (size_t index = 0; index < 20; ++index)
    {
        Guard<ThreadLocker> stdout_guard(s_stdout_locker);
        printf("[detached thread] %d\n", index);
    }

    s_stdout_locker.acquire();
    printf("[detached thread] end\n");
    s_stdout_locker.release();

    return(THREAD_DEFAULT_RET);
}

void test_base_thread(void)
{
    stupid_create_detached_thread(detached_thread_run, nullptr);

    TEST_THREAD thread_1 = { &s_thread_1, 1 };
    TEST_THREAD thread_2 = { &s_thread_2, 2 };

    s_thread_1.set_thread_args(thread_run, &thread_1, "thread 1");
    s_thread_2.set_thread_args(thread_run, &thread_2, "thread 2");

    s_thread_1.acquire();
    s_thread_2.acquire();

    stupid_ms_sleep(100);

    s_thread_1.release();
    s_thread_2.release();
}
