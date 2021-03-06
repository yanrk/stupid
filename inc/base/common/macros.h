/********************************************************
 * Description : macros of stupid base
 * Data        : 2013-05-18 17:10:05
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_MACROS_H
#define STUPID_BASE_MACROS_H


#ifdef _MSC_VER
    #define STUPID_CDECL           __cdecl
    #define STUPID_STDCALL         __stdcall
    #ifdef EXPORT_STUPID_DLL
        #define STUPID_API         __declspec(dllexport)
    #else
        #ifdef USE_STUPID_DLL
            #define STUPID_API     __declspec(dllimport)
        #else
            #define STUPID_API
        #endif // USE_STUPID_DLL
    #endif // EXPORT_STUPID_DLL
#else
    #define STUPID_CDECL
    #define STUPID_STDCALL
    #define STUPID_API
#endif // _MSC_VER

#ifdef __cplusplus
    #define STUPID_C_API(return_type)            extern "C" STUPID_API return_type STUPID_CDECL
    #define STUPID_EXTERN_TYPE(variable_type)    extern "C" STUPID_API variable_type
#else
    #define STUPID_C_API(return_type)            extern     STUPID_API return_type STUPID_CDECL
    #define STUPID_EXTERN_TYPE(variable_type)    extern     STUPID_API variable_type
#endif // __cplusplus

#define STUPID_CXX_API(return_type)              extern     STUPID_API return_type STUPID_CDECL

#ifdef __cplusplus
    #define STUPID_INLINE             inline
    #define STUPID_DEFAULT_ARG(var)   = var
#else
    #define STUPID_INLINE             static
    #define STUPID_DEFAULT_ARG(var)
#endif // __cplusplus

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    #define STUPID_I64_VAL(n)   n##I64
    #define STUPID_U64_VAL(n)   n##UI64
    #define STUPID_U64_FMT      "%I64u"
    #define STUPID_I64_FMT      "%I64d"
    #define STUPID_I64_ARG(x)   ((__int64)(x))
    #define STUPID_U64_ARG(x)   ((unsigned __int64)(x))
#else
    #define STUPID_I64_VAL(n)   n##LL
    #define STUPID_U64_VAL(n)   n##ULL
    #define STUPID_U64_FMT      "%llu"
    #define STUPID_I64_FMT      "%lld"
    #define STUPID_I64_ARG(x)   ((long long)(x))
    #define STUPID_U64_ARG(x)   ((unsigned long long)(x))
#endif // defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)

#if (defined(__APPLE__) && defined(__GNUC__)) || defined(__MACOSX__) || defined(macintosh)
    #define _MAC_OS
#else
    #undef  _MAC_OS
#endif // (defined(__APPLE__) && defined(__GNUC__)) || defined(__MACOSX__) || defined(macintosh)

#ifdef _MSC_VER
    #define STUPID_LIBRARY(path, library)   path "/" library ".dll"
#elif defined(_MAC_OS)
    #define STUPID_LIBRARY(path, library)   path "/lib" library ".dylib"
#else
    #define STUPID_LIBRARY(path, library)   path "/lib" library ".so"
#endif // _MSC_VER

#ifdef _MSC_VER
    #define stupid_system_error()                (::GetLastError())
#else
    #define stupid_system_error()                (errno + 0)
#endif // _MSC_VER


#endif // STUPID_BASE_MACROS_H
