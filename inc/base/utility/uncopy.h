/********************************************************
 * Description : base class which not support copy
 * Data        : 2013-05-20 12:24:45
 * Author      : yanrk
 * Email       : feeling_dxl@yeah.net & ken_scott@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2015
 ********************************************************/

#ifndef STUPID_BASE_UNCOPY_H
#define STUPID_BASE_UNCOPY_H


#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

class STUPID_API Uncopy
{
public:
    Uncopy();
    ~Uncopy();

private:
    Uncopy(const Uncopy &);
    Uncopy & operator = (const Uncopy &);
};

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_UNCOPY_H
