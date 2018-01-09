/********************************************************
 * Description : make sure program only runs an entity
 * Data        : 2015-07-02 10:55:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_EXCLUSIVE_H
#define STUPID_BASE_EXCLUSIVE_H


#include "base/common/common.h"
#include "base/utility/uncopy.h"
#include "base/utility/singleton.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API MutexApplication : private Uncopy
{
public:
    bool exclusive(const char * exclusive_unique_name, bool global_exclusive = true);

private:
    MutexApplication();
    ~MutexApplication();

private:
    friend class Singleton<MutexApplication>;

private:
    bool                        m_exclusive;
    size_t                      m_unique_id;
};

STUPID_CXX_API(bool) mutex_application_is_exist(const char * exclusive_unique_name, bool global_exclusive = true);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_EXCLUSIVE_H
