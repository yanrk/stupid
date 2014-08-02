/********************************************************
 * Description : singleton template of boost
 * Data        : 2013-10-19 09:14:45
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_SINGLETON_H
#define STUPID_BASE_SINGLETON_H


#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

template <typename T>
class Singleton
{
public:
    static T & instance()
    {
        static T obj;
        creator.do_nothing();
        return(obj);
    }

private:
    struct object_creator
    {
        object_creator()
        {
            Singleton<T>::instance();
        }

        inline void do_nothing() const { }
    };

    static object_creator creator;
};

template <typename T> typename Singleton<T>::object_creator Singleton<T>::creator;

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_SINGLETON_H
