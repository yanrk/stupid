/********************************************************
 * Description : hardware information of system
 * Data        : 2016-04-06 16:56:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef STUPID_BASE_HARDWARE_H
#define STUPID_BASE_HARDWARE_H


#include <cstddef>
#include <cstdint>
#include <vector>
#include "base/common/common.h"

NAMESPACE_STUPID_BASE_BEGIN

struct ifconfig_t
{
    char ip[16];
    char mask[16];
    char broadcast[16];
    char mac[18];
    char name[132];
};

STUPID_CXX_API(bool) get_system_ifconfig(std::vector<ifconfig_t> & ifconfigs);
STUPID_CXX_API(bool) get_system_memory_usage(uint64_t & total_size, uint64_t & avali_size);
STUPID_CXX_API(bool) get_system_disk_usage(const char * disk_path, uint64_t & total_size, uint64_t & avali_size);
STUPID_CXX_API(bool) get_system_cpu_usage(std::vector<size_t> & cpu_usage);
STUPID_CXX_API(bool) get_system_cpu_core_count(size_t & core_count);
STUPID_CXX_API(bool) get_system_uptime(uint64_t & uptime);

NAMESPACE_STUPID_BASE_END


#endif // STUPID_BASE_HARDWARE_H
