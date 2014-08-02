#include "test_base.h"
#include "base/utility/id_manager.h"

USING_NAMESPACE_STUPID_BASE

void test_base_id_manager(void)
{
    int min = 0;
    int max = 20;
    IDManager<int, NullLocker> id_manager(min, max);

    if (!id_manager.release(min))
    {
        std::cout << "as i known, release " << min << " failed" << std::endl;
    }
    else
    {
        assert(false);
    }

    int handle = 0;
    while (id_manager.acquire(handle))
    {
        std::cout << "acquire " << handle << std::endl;
    }

    for (int i = min; i < max; i += 2)
    {
        id_manager.release(i);
        std::cout << "release " << i << std::endl;
    }

    if (!id_manager.release(min))
    {
        std::cout << "as i known, release " << min << " failed" << std::endl;
    }
    else
    {
        assert(false);
    }

    if (id_manager.acquire(handle))
    {
        std::cout << "acquire " << handle << std::endl;
        id_manager.release(handle);
        std::cout << "release " << handle << std::endl;
    }

    for (int i = max - 1; i > min; i -= 2)
    {
        id_manager.release(i);
        std::cout << "release " << i << std::endl;
    }

    if (!id_manager.release(min))
    {
        std::cout << "as i known, release " << min << " failed" << std::endl;
    }
    else
    {
        assert(false);
    }

    if (id_manager.acquire(handle) && min == handle)
    {
        std::cout << "acquire " << handle << std::endl;
    }
    else
    {
        assert(false);
    }
}
