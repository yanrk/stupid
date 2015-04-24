#include "test_base.h"
#include "base/utility/bitmap.h"

USING_NAMESPACE_STUPID_BASE

void test_base_bitmap(void)
{
    int arr[] = { 9, 5, 4, 6, 7, 8, 0, 1, 55, -100 };

    int min = arr[0];
    int max = arr[0];
    std::cout << "array: ";
    for (size_t i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
        else if (arr[i] < min)
        {
            min = arr[i];
        }
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    BitMap<int> bitmap(min, max);
    for (size_t i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
    {
        bitmap.set(arr[i]);
    }

    std::cout << "sorted:";
    for (int val = min; val <= max; ++val)
    {
        if (bitmap.test(val))
        {
            std::cout << val << ' ';
        }
    }
    std::cout << std::endl;
}
