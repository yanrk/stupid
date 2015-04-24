#include "test_base.h"
#include "simple_math.h"
#include "base/library/library.h"

USING_NAMESPACE_STUPID_BASE

typedef int (*simple_math_calc_func) (int, int);
typedef simple_math * (*create_math_func) (int);
typedef void (*destroy_math_func) (simple_math *);

void test_base_library_1(void)
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

        create_math_func create_math = reinterpret_cast<create_math_func>(stupid_library_symbol(library, "create_math"));
        if (nullptr == create_math)
        {
            printf("%s\n", stupid_library_error());
            break;
        }

        destroy_math_func destroy_math = reinterpret_cast<destroy_math_func>(stupid_library_symbol(library, "destroy_math"));
        if (nullptr == destroy_math)
        {
            printf("%s\n", stupid_library_error());
            break;
        }

        simple_math * math_ptr = create_math(95);
        if (nullptr == math_ptr)
        {
            printf("create_math failed\n");
            break;
        }

        {
            int a = math_ptr->get();
            int b = 2;
            int c = math_ptr->add_to(b);
            printf("%d += %d is %d\n", a, b, c);
        }

        {
            int a = math_ptr->get();
            int b = 3;
            int c = math_ptr->sub_to(b);
            printf("%d -= %d is %d\n", a, b, c);
        }

        {
            int a = math_ptr->get();
            int b = 4;
            int c = math_ptr->mul_to(b);
            printf("%d *= %d is %d\n", a, b, c);
        }

        {
            int a = math_ptr->get();
            int b = 5;
            int c = math_ptr->div_to(b);
            printf("%d /= %d is %d\n", a, b, c);
        }

        {
            int a = math_ptr->get();
            int b = 6;
            int c = math_ptr->mod_to(b);
            printf("%d %%= %d is %d\n", a, b, c);
        }

        destroy_math(math_ptr);
        math_ptr = nullptr;
    } while (false);

    if (0 == stupid_library_release(library))
    {
        printf("%s\n", stupid_library_error());
        return;
    }
}

void test_base_library_2(void)
{
    Library library;
    if (!library.acquire(STUPID_LIBRARY(".", "simple_math")))
    {
        printf("%s\n", library.what().c_str());
        return;
    }

    do
    {
        simple_math_calc_func simple_add = library.get<simple_math_calc_func>("simple_add");
        if (nullptr == simple_add)
        {
            printf("%s\n", library.what().c_str());
            break;
        }
        simple_math_calc_func simple_sub = library.get<simple_math_calc_func>("simple_sub");
        if (nullptr == simple_sub)
        {
            printf("%s\n", library.what().c_str());
            break;
        }
        simple_math_calc_func simple_mul = library.get<simple_math_calc_func>("simple_mul");
        if (nullptr == simple_mul)
        {
            printf("%s\n", library.what().c_str());
            break;
        }
        simple_math_calc_func simple_div = library.get<simple_math_calc_func>("simple_div");
        if (nullptr == simple_div)
        {
            printf("%s\n", library.what().c_str());
            break;
        }
        simple_math_calc_func simple_mod = library.get<simple_math_calc_func>("simple_mod");
        if (nullptr == simple_mod)
        {
            printf("%s\n", library.what().c_str());
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

        create_math_func create_math = library.get<create_math_func>("create_math");
        if (nullptr == create_math)
        {
            printf("%s\n", library.what().c_str());
            break;
        }

        destroy_math_func destroy_math = library.get<destroy_math_func>("destroy_math");
        if (nullptr == destroy_math)
        {
            printf("%s\n", library.what().c_str());
            break;
        }

        simple_math * math_ptr = create_math(95);
        if (nullptr == math_ptr)
        {
            printf("create_math failed\n");
            break;
        }

        {
            int a = math_ptr->get();
            int b = 2;
            int c = math_ptr->add_to(b);
            printf("%d += %d is %d\n", a, b, c);
        }

        {
            int a = math_ptr->get();
            int b = 3;
            int c = math_ptr->sub_to(b);
            printf("%d -= %d is %d\n", a, b, c);
        }

        {
            int a = math_ptr->get();
            int b = 4;
            int c = math_ptr->mul_to(b);
            printf("%d *= %d is %d\n", a, b, c);
        }

        {
            int a = math_ptr->get();
            int b = 5;
            int c = math_ptr->div_to(b);
            printf("%d /= %d is %d\n", a, b, c);
        }

        {
            int a = math_ptr->get();
            int b = 6;
            int c = math_ptr->mod_to(b);
            printf("%d %%= %d is %d\n", a, b, c);
        }

        destroy_math(math_ptr);
        math_ptr = nullptr;
    } while (false);

    library.release();
}

void test_base_library(void)
{
    std::cout << "---- test library with c functions ----" << std::endl;
    test_base_library_1();
    std::cout << "----  test library with cxx class  ----" << std::endl;
    test_base_library_2();
    std::cout << std::endl;
}
