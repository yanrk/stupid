/********************************************************
 * Description : macros of namespace
 * Data        : 2013-05-18 17:51:57
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_NAMESPACE_H
#define STUPID_BASE_NAMESPACE_H


#ifdef __cplusplus
    namespace Stupid
    {
        namespace Base
        {

        }
    }

    #define NAMESPACE_STUPID_BASE_BEGIN   \
    namespace Stupid                      \
    {                                     \
        namespace Base                    \
        {

    #define NAMESPACE_STUPID_BASE_END     \
        }                                 \
    }

    #define USING_NAMESPACE_STUPID_BASE   \
    using namespace Stupid::Base;
#else
    #define NAMESPACE_STUPID_BASE_BEGIN
    #define NAMESPACE_STUPID_BASE_END
    #define USING_NAMESPACE_STUPID_BASE
#endif // __cplusplus


#endif // STUPID_BASE_NAMESPACE_H
