/********************************************************
 * Description : library dynamic import functions
 * Data        : 2015-03-24 09:22:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif // _MSC_VER

#include <cstdio>

#include "base/library/library.h"
#include "base/string/string.h"

NAMESPACE_STUPID_BASE_BEGIN

void * stupid_library_acquire(const char * filename)
{
    if (nullptr == filename)
    {
        printf("filename is nullptr\n");
        return(nullptr);
    }

#ifdef _MSC_VER
    return(reinterpret_cast<void *>(LoadLibraryA(filename)));
#else
    return(dlopen(filename, RTLD_LAZY | RTLD_LOCAL));
#endif // _MSC_VER
}

int stupid_library_release(void * library)
{
    if (nullptr == library)
    {
        printf("library is nullptr\n");
        return(0);
    }

#ifdef _MSC_VER
    return(FreeLibrary(reinterpret_cast<HMODULE>(library)) ? 1 : 0);
#else
    return(0 == dlclose(library) ? 1 : 0);
#endif // _MSC_VER
}

void * stupid_library_symbol(void * library, const char * symbol)
{
    if (nullptr == library)
    {
        printf("library is nullptr\n");
        return(nullptr);
    }

    if (nullptr == symbol)
    {
        printf("symbol is nullptr\n");
        return(nullptr);
    }

#ifdef _MSC_VER
    return(GetProcAddress(reinterpret_cast<HMODULE>(library), symbol));
#else
    return(dlsym(library, symbol));
#endif // _MSC_VER
}

const char * stupid_library_error(void)
{
    static char library_error[128] = { 0 };
#ifdef _MSC_VER
    DWORD error_code = GetLastError();
    if (0 == error_code)
    {
        return("");
    }
    stupid_snprintf(library_error, sizeof(library_error), "library error: %d", error_code);
#else
    const char * error_info = dlerror();
    if (nullptr == error_info)
    {
        return("");
    }
    stupid_snprintf(library_error, sizeof(library_error), "library error: %s", error_info);
#endif // _MSC_VER
    return(library_error);
}

Library::Library()
    : m_name()
    , m_what()
    , m_library(nullptr)
{

}

Library::~Library()
{
    release();
}

bool Library::acquire(const char * library_name)
{
    release();

    if (nullptr == library_name)
    {
        m_what = "library name is nullptr";
        return(false);
    }

    m_name = library_name;
    m_library = stupid_library_acquire(library_name);
    if (nullptr == m_library)
    {
        m_what = stupid_library_error();
        return(false);
    }

    return(true);
}

void Library::release()
{
    m_name.clear();
    m_what.clear();
    if (nullptr != m_library)
    {
        if (0 == stupid_library_release(m_library))
        {
            m_what = stupid_library_error();
        }
        m_library = 0;
    }
}

const std::string & Library::name() const
{
    return(m_name);
}

const std::string & Library::what() const
{
    return(m_what);
}

NAMESPACE_STUPID_BASE_END
