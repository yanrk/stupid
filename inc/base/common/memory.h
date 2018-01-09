/********************************************************
 * Description : alloc and free memory macros
 * Data        : 2013-05-18 18:00:25
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_MEMORY_H
#define STUPID_BASE_MEMORY_H


#define STUPID_NEW(var, type)             \
do                                        \
{                                         \
    try                                   \
    {                                     \
        (var) = new type;                 \
    }                                     \
    catch (...)                           \
    {                                     \
        (var) = nullptr;                  \
    }                                     \
} while (false)

#define STUPID_NEW_ARR(var, type, size)   \
do                                        \
{                                         \
    try                                   \
    {                                     \
        (var) = new type[size];           \
    }                                     \
    catch (...)                           \
    {                                     \
        (var) = nullptr;                  \
    }                                     \
} while (false)

#define STUPID_DEL(var)                   \
do                                        \
{                                         \
    try                                   \
    {                                     \
        delete (var);                     \
    }                                     \
    catch (...)                           \
    {                                     \
                                          \
    }                                     \
    (var) = nullptr;                      \
} while (false)

#define STUPID_DEL_ARR(var)               \
do                                        \
{                                         \
    try                                   \
    {                                     \
        delete [] (var);                  \
    }                                     \
    catch (...)                           \
    {                                     \
                                          \
    }                                     \
    (var) = nullptr;                      \
} while (false)


#endif // STUPID_BASE_MEMORY_H
