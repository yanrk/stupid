#include "test_base.h"
#include "base/utility/guard.h"
#include "base/thread/thread.h"
#include "base/locker/locker.h"
#include "base/process/process.h"
#include "base/time/time.h"

USING_NAMESPACE_STUPID_BASE

static Process        s_process;
static ThreadLocker   s_stdout_locker;

static thread_return_t STUPID_STDCALL process_wait_exit_thread_run(thread_argument_t argument)
{
    s_stdout_locker.acquire();
    printf("process wait exit thread start\n");
    s_stdout_locker.release();

    int exit_code = 0;
    if (s_process.wait_exit(exit_code))
    {
        s_stdout_locker.acquire();
        printf("process wait exit success: %d\n", exit_code);
        s_stdout_locker.release();
    }
    else
    {
        s_stdout_locker.acquire();
        printf("process wait exit failure\n");
        s_stdout_locker.release();
    }

    s_stdout_locker.acquire();
    printf("process wait exit thread end\n");
    s_stdout_locker.release();

    return(THREAD_DEFAULT_RET);
}

static thread_return_t STUPID_STDCALL process_release_thread_run(thread_argument_t argument)
{
    s_stdout_locker.acquire();
    printf("process release thread start\n");
    s_stdout_locker.release();

    for (int index = 9; index >= 0; --index)
    {
        stupid_ms_sleep(1000);
        Guard<ThreadLocker> stdout_guard(s_stdout_locker);
        printf("process release thread waiting %d\n", index);
    }
    s_process.release();

    s_stdout_locker.acquire();
    printf("process release thread end\n");
    s_stdout_locker.release();

    return(THREAD_DEFAULT_RET);
}

void test_base_process(void)
{
#ifdef _MSC_VER
    const std::string command_line("ping -n 200 127.0.0.1");
#else
    const std::string command_line("ping -c 200 127.0.0.1");
#endif // _MSC_VER

    stupid_create_detached_process(command_line);

    s_process.set_process_args(command_line, "sub process");
    s_process.acquire();

    Thread thread_1;
    Thread thread_2;

    thread_1.set_thread_args(process_wait_exit_thread_run);
    thread_2.set_thread_args(process_release_thread_run);

    thread_1.acquire();
    thread_2.acquire();

    thread_1.release();
    thread_2.release();
}
