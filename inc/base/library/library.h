/********************************************************
 * Description : library dynamic import functions & class
 * Data        : 2015-03-24 09:22:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_LIBRARY_H
#define STUPID_BASE_LIBRARY_H


#include <string>
#include "base/common/common.h"
#include "base/utility/uncopy.h"

NAMESPACE_STUPID_BASE_BEGIN

STUPID_C_API(void *) stupid_library_acquire(const char * filename); /* null: failure */
STUPID_C_API(int) stupid_library_release(void * library); /* 1: success, 0: failure */
STUPID_C_API(void *) stupid_library_symbol(void * library, const char * symbol); /* null: failure */
STUPID_C_API(const char *) stupid_library_error(void); /* "": success */

class STUPID_API Library : private Uncopy
{
public:
    Library();
    ~Library();

public:
    bool acquire(const char * library_name);
    void release();

public:
    const std::string & name() const;
    const std::string & what() const;

public:
    template <typename FuncPtr> FuncPtr get(const char * function_name);

private:
    std::string          m_name;
    std::string          m_what;
    void               * m_library;
};

template <typename FuncPtr>
FuncPtr Library::get(const char * function_name)
{
    if (nullptr == m_library)
    {
        m_what = "library is uninitialized";
        return nullptr;
    }

    if (nullptr == function_name)
    {
        m_what = "function name is nullptr";
        return nullptr;
    }

    void * func_ptr = stupid_library_symbol(m_library, function_name);
    if (nullptr == func_ptr)
    {
        m_what = stupid_library_error();
        return nullptr;
    }

    m_what.clear();
    return reinterpret_cast<FuncPtr>(func_ptr);
}

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_LIBRARY_H
