#include "test_base.h"
#include "base/pool/object_pool.h"

USING_NAMESPACE_STUPID_BASE

void test_base_object_pool(void)
{
    ObjectPool<int> int_pool_1;
    int * i = int_pool_1.acquire();
    int_pool_1.release(i);
    assert(nullptr == i);

    std::list<int *> int_list;
    int_pool_1 >> int_list;
    int_pool_1 << int_list;
    int_list << int_pool_1;
    int_list >> int_pool_1;
    assert(int_list.empty());

    ObjectPool<int> int_pool_2;
    int_pool_2 << int_pool_1;
    int_pool_2 >> int_pool_1;

    assert(1 == int_pool_1.size());
    assert(0 == int_pool_2.size());
}
