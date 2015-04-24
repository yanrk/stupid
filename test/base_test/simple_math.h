#ifndef SIMPLE_MATH_H
#define SIMPLE_MATH_H


#ifdef _MSC_VER
    #define MY_CDECL     __cdecl
    #ifdef EXPORT_DLL
        #define MY_API   __declspec(dllexport)
    #else
        #define MY_API   __declspec(dllimport)
    #endif // EXPORT_DLL
#else
    #define MY_CDECL
    #define MY_API
#endif // _MSC_VER

#ifdef __cplusplus
    #define MY_C_API(ret_type)    extern "C" MY_API ret_type MY_CDECL
#else
    #define MY_C_API(ret_type)    extern     MY_API ret_type MY_CDECL
#endif // __cplusplus


MY_C_API(int) simple_add(int, int);
MY_C_API(int) simple_sub(int, int);
MY_C_API(int) simple_mul(int, int);
MY_C_API(int) simple_div(int, int);
MY_C_API(int) simple_mod(int, int);


class MY_API simple_math
{
public:
    virtual ~simple_math() = 0;

public:
    virtual int get() const = 0;

public:
    virtual int add_to(int) = 0;
    virtual int sub_to(int) = 0;
    virtual int mul_to(int) = 0;
    virtual int div_to(int) = 0;
    virtual int mod_to(int) = 0;
};

MY_C_API(simple_math *) create_math(int val);
MY_C_API(void) destroy_math(simple_math * ptr);


#endif // SIMPLE_MATH_H
