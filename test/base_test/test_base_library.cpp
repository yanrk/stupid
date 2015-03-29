#include "test_base.h"
#include "base/library/library.h"

USING_NAMESPACE_STUPID_BASE

typedef int (*simple_math_calc_func) (int, int);

void test_base_library(void)
{
    void * library = stupid_library_acquire(STUPID_LIBRARY(".", "simple_math"));
    if (nullptr == library)
    {
        printf("%s\n", stupid_library_error());
        return;
    }

    do
    {
        simple_math_calc_func simple_add = reinterpret_cast<simple_math_calc_func>(stupid_library_symbol(library, "simple_add"));
        if (nullptr == simple_add)
        {
            printf("%s\n", stupid_library_error());
            break;
        }
        simple_math_calc_func simple_sub = reinterpret_cast<simple_math_calc_func>(stupid_library_symbol(library, "simple_sub"));
        if (nullptr == simple_sub)
        {
            printf("%s\n", stupid_library_error());
            break;
        }
        simple_math_calc_func simple_mul = reinterpret_cast<simple_math_calc_func>(stupid_library_symbol(library, "simple_mul"));
        if (nullptr == simple_mul)
        {
            printf("%s\n", stupid_library_error());
            break;
        }
        simple_math_calc_func simple_div = reinterpret_cast<simple_math_calc_func>(stupid_library_symbol(library, "simple_div"));
        if (nullptr == simple_div)
        {
            printf("%s\n", stupid_library_error());
            break;
        }
        simple_math_calc_func simple_mod = reinterpret_cast<simple_math_calc_func>(stupid_library_symbol(library, "simple_mod"));
        if (nullptr == simple_mod)
        {
            printf("%s\n", stupid_library_error());
            break;
        }

        {
            int a = 3;
            int b = 5;
            printf("%d + %d = %d\n", a, b, simple_add(a, b));
            printf("%d - %d = %d\n", a, b, simple_sub(a, b));
            printf("%d * %d = %d\n", a, b, simple_mul(a, b));
            printf("%d / %d = %d\n", a, b, simple_div(a, b));
            printf("%d %% %d = %d\n", a, b, simple_mod(a, b));
            printf("\n");
        }

        {
            int a = 5;
            int b = 3;
            printf("%d + %d = %d\n", a, b, simple_add(a, b));
            printf("%d - %d = %d\n", a, b, simple_sub(a, b));
            printf("%d * %d = %d\n", a, b, simple_mul(a, b));
            printf("%d / %d = %d\n", a, b, simple_div(a, b));
            printf("%d %% %d = %d\n", a, b, simple_mod(a, b));
            printf("\n");
        }

        {
            int a = -3;
            int b = 5;
            printf("%d + %d = %d\n", a, b, simple_add(a, b));
            printf("%d - %d = %d\n", a, b, simple_sub(a, b));
            printf("%d * %d = %d\n", a, b, simple_mul(a, b));
            printf("%d / %d = %d\n", a, b, simple_div(a, b));
            printf("%d %% %d = %d\n", a, b, simple_mod(a, b));
            printf("\n");
        }

        {
            int a = 5;
            int b = -3;
            printf("%d + %d = %d\n", a, b, simple_add(a, b));
            printf("%d - %d = %d\n", a, b, simple_sub(a, b));
            printf("%d * %d = %d\n", a, b, simple_mul(a, b));
            printf("%d / %d = %d\n", a, b, simple_div(a, b));
            printf("%d %% %d = %d\n", a, b, simple_mod(a, b));
            printf("\n");
        }

        {
            int a = 3;
            int b = -5;
            printf("%d + %d = %d\n", a, b, simple_add(a, b));
            printf("%d - %d = %d\n", a, b, simple_sub(a, b));
            printf("%d * %d = %d\n", a, b, simple_mul(a, b));
            printf("%d / %d = %d\n", a, b, simple_div(a, b));
            printf("%d %% %d = %d\n", a, b, simple_mod(a, b));
            printf("\n");
        }

        {
            int a = -5;
            int b = 3;
            printf("%d + %d = %d\n", a, b, simple_add(a, b));
            printf("%d - %d = %d\n", a, b, simple_sub(a, b));
            printf("%d * %d = %d\n", a, b, simple_mul(a, b));
            printf("%d / %d = %d\n", a, b, simple_div(a, b));
            printf("%d %% %d = %d\n", a, b, simple_mod(a, b));
            printf("\n");
        }

        {
            int a = -3;
            int b = -5;
            printf("%d + %d = %d\n", a, b, simple_add(a, b));
            printf("%d - %d = %d\n", a, b, simple_sub(a, b));
            printf("%d * %d = %d\n", a, b, simple_mul(a, b));
            printf("%d / %d = %d\n", a, b, simple_div(a, b));
            printf("%d %% %d = %d\n", a, b, simple_mod(a, b));
            printf("\n");
        }

        {
            int a = -5;
            int b = -3;
            printf("%d + %d = %d\n", a, b, simple_add(a, b));
            printf("%d - %d = %d\n", a, b, simple_sub(a, b));
            printf("%d * %d = %d\n", a, b, simple_mul(a, b));
            printf("%d / %d = %d\n", a, b, simple_div(a, b));
            printf("%d %% %d = %d\n", a, b, simple_mod(a, b));
            printf("\n");
        }
    } while (false);

    if (0 == stupid_library_release(library))
    {
        printf("%s\n", stupid_library_error());
        return;
    }
}
