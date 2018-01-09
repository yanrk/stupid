/********************************************************
 * Description : macros of namespace
 * Data        : 2014-04-20 21:40:12
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_TOOL_NAMESPACE_H
#define STUPID_TOOL_NAMESPACE_H


#ifdef __cplusplus
    namespace Stupid
    {
        namespace Tool
        {

        }
    }

    #define NAMESPACE_STUPID_TOOL_BEGIN   \
    namespace Stupid                      \
    {                                     \
        namespace Tool                    \
        {

    #define NAMESPACE_STUPID_TOOL_END     \
        }                                 \
    }

    #define USING_NAMESPACE_STUPID_TOOL   \
    using namespace Stupid::Tool;
#else
    #define NAMESPACE_STUPID_TOOL_BEGIN
    #define NAMESPACE_STUPID_TOOL_END
    #define USING_NAMESPACE_STUPID_TOOL
#endif // __cplusplus


#endif // STUPID_TOOL_NAMESPACE_H
