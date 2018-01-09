/********************************************************
 * Description : macros of namespace
 * Data        : 2013-10-29 23:44:45
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_NET_NAMESPACE_H
#define STUPID_NET_NAMESPACE_H


#ifdef __cplusplus
    namespace Stupid
    {
        namespace Net
        {

        }
    }

    #define NAMESPACE_STUPID_NET_BEGIN    \
    namespace Stupid                      \
    {                                     \
        namespace Net                     \
        {

    #define NAMESPACE_STUPID_NET_END      \
        }                                 \
    }

    #define USING_NAMESPACE_STUPID_NET    \
    using namespace Stupid::Net;
#else
    #define NAMESPACE_STUPID_NET_BEGIN
    #define NAMESPACE_STUPID_NET_END
    #define USING_NAMESPACE_STUPID_NET
#endif // __cplusplus


#endif // STUPID_NET_NAMESPACE_H
