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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "base/charset/charset.h"
#include "base/filesystem/hardware.h"

#ifdef _MSC_VER

#include <ctime>
#include <atlstr.h>
#include <winsock2.h>
#include <iphlpapi.h>

typedef DWORD (__stdcall * NtQuerySystemInformationFuncPtr)(DWORD, PVOID, DWORD, PDWORD);

#pragma pack(push,8)

typedef enum _SYSTEMINFOCLASS
{
    SystemProcessorPerformanceInformation = 8
} SYSTEMINFOCLASS, * PSYSTEMINFOCLASS;

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
{
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER DpcTime;
    LARGE_INTEGER InterruptTime;
    DWORD InterruptCount;
    DWORD dwUnknown1;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, * PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

#pragma pack(pop)

NAMESPACE_STUPID_BASE_BEGIN

bool get_system_ifconfig(std::vector<ifconfig_t> & ifconfigs)
{
    ULONG addr_buffer_size = 0;
    GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &addr_buffer_size);
    if (0 == addr_buffer_size)
    {
        return false;
    }

    std::vector<char> addr_buffer(addr_buffer_size, 0x0);
    IP_ADAPTER_ADDRESSES * addresses = reinterpret_cast<IP_ADAPTER_ADDRESSES *>(&addr_buffer[0]);
    GetAdaptersAddresses(AF_INET, 0, nullptr, addresses, &addr_buffer_size);

    ULONG info_buffer_size = 0;
    GetAdaptersInfo(nullptr, &info_buffer_size);
    if (0 == info_buffer_size)
    {
        return false;
    }

    std::vector<char> info_buffer(info_buffer_size, 0x0);
    IP_ADAPTER_INFO * informations = reinterpret_cast<IP_ADAPTER_INFO *>(&info_buffer[0]);
    GetAdaptersInfo(informations, &info_buffer_size);

    for (IP_ADAPTER_ADDRESSES * addr = addresses; nullptr != addr; addr = addr->Next)
    {
        for (IP_ADAPTER_INFO * info = informations; nullptr != info; info = info->Next)
        {
            if (0 == strcmp(addr->AdapterName, info->AdapterName))
            {
                ifconfig_t ifconfig = { 0x0 };

                strncpy(ifconfig.ip, info->IpAddressList.IpAddress.String, sizeof(ifconfig.ip) - 1);
                if (0 == strcmp("0.0.0.0", ifconfig.ip) || 0 == strcmp("127.0.0.1", ifconfig.ip))
                {
                    continue;
                }

                unsigned char * mac = reinterpret_cast<unsigned char *>(addr->PhysicalAddress);
                snprintf(ifconfig.mac, sizeof(ifconfig.mac), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                if (0 == strcmp("00:00:00:00:00:00", ifconfig.mac))
                {
                    continue;
                }

                strncpy(ifconfig.mask, info->IpAddressList.IpMask.String, sizeof(ifconfig.mask) - 1);

                const char * ip = ifconfig.ip;
                const char * mask = ifconfig.mask;
                char * broadcast = ifconfig.broadcast;
                while ('\0' != *ip && '\0' != *mask)
                {
                    if (0 == strncmp("255", mask, 3))
                    {
                        while ('\0' != *mask && '.' != *mask)
                        {
                            mask++;
                        }

                        if ('.' == *mask)
                        {
                            mask++;
                        }

                        while ('\0' != *ip && '.' != *ip)
                        {
                            *broadcast++ = *ip++;
                        }

                        if ('.' == *ip)
                        {
                            *broadcast++ = *ip++;
                        }
                    }
                    else
                    {
                        strcpy(broadcast, "255");
                        broadcast += 3;

                        while ('\0' != *mask && '.' != *mask)
                        {
                            mask++;
                        }

                        if ('.' == *mask)
                        {
                            mask++;
                        }

                        while ('\0' != *ip && '.' != *ip)
                        {
                            ip++;
                        }

                        if ('.' == *ip)
                        {
                            *broadcast++ = *ip++;
                        }
                    }
                }

                strncpy(ifconfig.name, info->Description, sizeof(ifconfig.name) - 1);

                ifconfigs.push_back(ifconfig);

                break;
            }
        }
    }

    return !ifconfigs.empty();
}

bool get_system_memory_usage(uint64_t & total_size, uint64_t & avali_size)
{
    MEMORYSTATUSEX memory_status_ex = { 0x00 };
    memory_status_ex.dwLength = sizeof(memory_status_ex);
    if (GlobalMemoryStatusEx(&memory_status_ex))
    {
        total_size = static_cast<uint64_t>(memory_status_ex.ullTotalPhys);
        avali_size = static_cast<uint64_t>(memory_status_ex.ullAvailPhys);
        return true;
    }
    else
    {
        total_size = 1;
        avali_size = 0;
        return false;
    }
}

bool get_system_disk_usage(const char * disk_path, uint64_t & total_size, uint64_t & avali_size)
{
    if (nullptr == disk_path)
    {
        return false;
    }

    ULARGE_INTEGER caller_bytes;
    ULARGE_INTEGER total_bytes;
    ULARGE_INTEGER avali_bytes;

    if (GetDiskFreeSpaceExW(utf8_to_unicode(disk_path).c_str(), &caller_bytes, &total_bytes, &avali_bytes))
    {
        total_size = static_cast<uint64_t>(total_bytes.QuadPart);
        avali_size = static_cast<uint64_t>(avali_bytes.QuadPart);
        return true;
    }
    else
    {
        total_size = 1;
        avali_size = 0;
        return false;
    }
}

bool get_system_cpu_usage(std::vector<size_t> & cpu_usage)
{
    cpu_usage.clear();

    const int max_core_count = 256;
    static bool first_time = true;
    static SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION old_sppi_array[max_core_count];

    HMODULE module = GetModuleHandle("ntdll.dll");
    if (nullptr == module)
    {
        return false;
    }

    NtQuerySystemInformationFuncPtr nt_query_system_information = (NtQuerySystemInformationFuncPtr)GetProcAddress(module, "NtQuerySystemInformation");
    if (nullptr == nt_query_system_information)
    {
        return false;
    }

    SYSTEM_INFO system_info = { 0x00 };
    GetSystemInfo(&system_info);

    DWORD query_core_count = (system_info.dwNumberOfProcessors < max_core_count ? system_info.dwNumberOfProcessors : max_core_count);

    SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION new_sppi_array[max_core_count];

    while (true)
    {
        unsigned long bytes_returned = 0;
        memset(new_sppi_array, 0x00, sizeof(new_sppi_array));

        if (0 != nt_query_system_information(SystemProcessorPerformanceInformation, new_sppi_array, (sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * query_core_count), &bytes_returned))
        {
            return false;
        }

        if (first_time)
        {
            first_time = false;
            memcpy(old_sppi_array, new_sppi_array, sizeof(old_sppi_array));
            Sleep(1000);
        }
        else
        {
            break;
        }
    }

    for (size_t index = 0; index < query_core_count; index++)
    {
        const SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION & new_sppi = new_sppi_array[index];
        const SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION & old_sppi = old_sppi_array[index];
        ULONGLONG top = (new_sppi.IdleTime.QuadPart - old_sppi.IdleTime.QuadPart) * 100;
        ULONGLONG bottom = (new_sppi.KernelTime.QuadPart + new_sppi.UserTime.QuadPart) - (old_sppi.KernelTime.QuadPart + old_sppi.UserTime.QuadPart);

        if (0 == bottom)
        {
            bottom = 1;
        }

        cpu_usage.push_back(static_cast<size_t>(100 - top / bottom));
    }

    memcpy(old_sppi_array, new_sppi_array, sizeof(old_sppi_array));

    return true;
}

bool get_system_cpu_core_count(size_t & core_count)
{
#if _WIN32_WINNT >= 0x0601
    core_count = 0;

    ULONG highest_node_number = 0;
    if (!GetNumaHighestNodeNumber(&highest_node_number))
    {
        return false;
    }

    for (USHORT node_number = 0; node_number <= highest_node_number; ++node_number)
    {
        GROUP_AFFINITY processor_mask;
        if (!GetNumaNodeProcessorMaskEx(node_number, &processor_mask))
        {
            continue;
        }

        std::size_t mask = processor_mask.Mask;
        while (0 != mask)
        {
            mask &= mask - 1;
            ++core_count;
        }
    }
#else
    SYSTEM_INFO system_info = { 0x00 };

#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
    GetNativeSystemInfo(&system_info);
#else
    GetSystemInfo(&system_info);
#endif // _WIN32_WINNT >= _WIN32_WINNT_WINXP

    core_count = static_cast<size_t>(system_info.dwNumberOfProcessors);
#endif // _WIN32_WINNT >= 0x0601
    return true;
}

bool get_system_uptime(uint64_t & uptime)
{
    uptime = static_cast<uint64_t>(GetTickCount64() / CLOCKS_PER_SEC);
    return true;
}

NAMESPACE_STUPID_BASE_END

#elif defined(_MAC_OS)

#include <errno.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <net/if_dl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/statvfs.h>
#include <mach/mach.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach_types.h>
#include <mach/vm_statistics.h>
#include <mach/processor_info.h>

NAMESPACE_STUPID_BASE_BEGIN

bool get_system_ifconfig(std::vector<ifconfig_t> & ifconfigs)
{
    struct ifaddrs * ifas = nullptr;
    if (0 != getifaddrs(&ifas))
    {
        return false;
    }

    for (struct ifaddrs * ifap_inet = ifas; nullptr != ifap_inet; ifap_inet = ifap_inet->ifa_next)
    {
        if (AF_INET != ifap_inet->ifa_addr->sa_family)
        {
            continue;
        }

        ifconfig_t ifconfig = { 0x0 };

        struct sockaddr_in * ip_addr = reinterpret_cast<struct sockaddr_in *>(ifap_inet->ifa_addr);
        inet_ntop(AF_INET, &ip_addr->sin_addr, ifconfig.ip, sizeof(ifconfig.ip) - 1);
        if (0 == strcmp("0.0.0.0", ifconfig.ip) || 0 == strcmp("127.0.0.1", ifconfig.ip))
        {
            continue;
        }

        strncpy(ifconfig.mac, "00:00:00:00:00:00", sizeof(ifconfig.mac) - 1);

        for (struct ifaddrs * ifap_link = ifas; nullptr != ifap_link; ifap_link = ifap_link->ifa_next)
        {
            if (AF_LINK != ifap_link->ifa_addr->sa_family)
            {
                continue;
            }

            if (0 != strcmp(ifap_inet->ifa_name, ifap_link->ifa_name))
            {
                continue;
            }

            struct sockaddr_dl * dl_addr = reinterpret_cast<struct sockaddr_dl *>(ifap_link->ifa_addr);
            unsigned char * mac = reinterpret_cast<unsigned char *>(LLADDR(dl_addr));
            snprintf(ifconfig.mac, sizeof(ifconfig.mac), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

            break;
        }

        if (0 == strcmp("00:00:00:00:00:00", ifconfig.mac))
        {
            continue;
        }

        struct sockaddr_in * mask_addr = reinterpret_cast<struct sockaddr_in *>(ifap_inet->ifa_netmask);
        inet_ntop(AF_INET, &mask_addr->sin_addr, ifconfig.mask, sizeof(ifconfig.mask) - 1);

        struct sockaddr_in * broadcast_addr = reinterpret_cast<struct sockaddr_in *>(ifap_inet->ifa_broadaddr);
        inet_ntop(AF_INET, &broadcast_addr->sin_addr, ifconfig.broadcast, sizeof(ifconfig.broadcast) - 1);

        strncpy(ifconfig.name, ifap_inet->ifa_name, sizeof(ifconfig.name) - 1);

        ifconfigs.push_back(ifconfig);
    }

    freeifaddrs(ifas);

    return !ifconfigs.empty();
}

bool get_system_memory_usage(uint64_t & total_size, uint64_t & avali_size)
{
    int mib[] = { CTL_HW, HW_MEMSIZE };
    size_t total_size_len = sizeof(total_size);
    if (0 != sysctl(mib, sizeof(mib) / sizeof(mib[0]), &total_size, &total_size_len, nullptr, 0))
    {
        total_size = 1;
        avali_size = 0;
        return false;
    }

    vm_size_t page_size;
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);
    mach_port_t mach_port = mach_host_self();
    if (KERN_SUCCESS != host_page_size(mach_port, &page_size) || KERN_SUCCESS != host_statistics64(mach_port, HOST_VM_INFO, (host_info64_t)&vm_stats, &count))
    {
        avali_size = 0;
        return false;
    }
    else
    {
        avali_size = static_cast<uint64_t>(page_size) * static_cast<uint64_t>(vm_stats.free_count + vm_stats.purgeable_count + vm_stats.external_page_count);
        return true;
    }
}

bool get_system_disk_usage(const char * disk_path, uint64_t & total_size, uint64_t & avali_size)
{
    if (nullptr == disk_path)
    {
        total_size = 1;
        avali_size = 0;
        return false;
    }

    struct statvfs file_info;
    if (0 == statvfs(utf8_to_ansi(disk_path).c_str(), &file_info))
    {
        total_size = file_info.f_bsize * file_info.f_blocks;
        avali_size = file_info.f_bsize * file_info.f_bavail;
        return true;
    }
    else
    {
        total_size = 1;
        avali_size = 0;
        return false;
    }
}

bool get_system_cpu_usage(std::vector<size_t> & cpu_usage)
{
    cpu_usage.clear();

    static processor_info_array_t new_cpu_info = nullptr;
    static processor_info_array_t old_cpu_info = nullptr;
    static mach_msg_type_number_t new_cpu_info_size = 0;
    static mach_msg_type_number_t old_cpu_info_size = 0;
    natural_t core_count = 0;
    if (KERN_SUCCESS != host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &core_count, &new_cpu_info, &new_cpu_info_size))
    {
        return false;
    }

    for (natural_t index = 0; index < core_count; ++index)
    {
        size_t inuse = 0;
        size_t total = 0;
        if (nullptr != old_cpu_info)
        {
            inuse = ((new_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_USER] - old_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_USER]) + (new_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_SYSTEM] - old_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_SYSTEM]) + (new_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_NICE] - old_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_NICE]));
            total = inuse + (new_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_IDLE] - old_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_IDLE]);
        }
        else
        {
            inuse = new_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_USER] + new_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_SYSTEM] + new_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_NICE];
            total = inuse + new_cpu_info[(CPU_STATE_MAX * index) + CPU_STATE_IDLE];
        }
        if (0 == total)
        {
            total = 1;
        }
        cpu_usage.push_back(inuse * 100 / total);
    }

    if (nullptr != old_cpu_info)
    {
        vm_deallocate(mach_task_self(), (vm_address_t)old_cpu_info, sizeof(integer_t) * old_cpu_info_size);
    }

    old_cpu_info = new_cpu_info;
    old_cpu_info_size = new_cpu_info_size;

    new_cpu_info = nullptr;
    new_cpu_info_size = 0;

    return true;
}

bool get_system_cpu_core_count(size_t & core_count)
{
    size_t len = sizeof(core_count);
    int mib[] = { CTL_HW, HW_NCPU };
    if (0 != sysctl(mib, sizeof(mib) / sizeof(mib[0]), &core_count, &len, nullptr, 0))
    {
        core_count = 1;
        return false;
    }
    return true;
}

bool get_system_uptime(uint64_t & uptime)
{
    struct timeval boot_time;
    size_t len = sizeof(boot_time);
    int mib[] = { CTL_KERN, KERN_BOOTTIME };
    if (0 != sysctl(mib, sizeof(mib) / sizeof(mib[0]), &boot_time, &len, nullptr, 0))
    {
        uptime = 0;
        return false;
    }
    else
    {
        uptime = static_cast<uint64_t>(time(nullptr) - boot_time.tv_sec);
        return true;
    }
}

NAMESPACE_STUPID_BASE_END

#else // !defined(_MSC_VER) && !defined(_MAC_OS)

#include <errno.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <fstream>

struct mem_occupy_t
{
    char         name[32];
    uint64_t     size;
    char         unit[16];
};

static uint64_t memory_unit_bytes(const char * unit)
{
    if (nullptr != strchr(unit, 'k') || nullptr != strchr(unit, 'K'))
    {
        return 1024ULL;
    }
    else if (nullptr != strchr(unit, 'm') || nullptr != strchr(unit, 'M'))
    {
        return 1024ULL * 1024;
    }
    else if (nullptr != strchr(unit, 'g') || nullptr != strchr(unit, 'G'))
    {
        return 1024ULL * 1024 * 1024;
    }
    else if (nullptr != strchr(unit, 't') || nullptr != strchr(unit, 'T'))
    {
        return 1024ULL * 1024 * 1024 * 1024;
    }
    else
    {
        return 1ULL;
    }
}

struct cpu_occupy_t
{
    char         name[32];
    unsigned int user;
    unsigned int nice;
    unsigned int system;
    unsigned int idle;
};

NAMESPACE_STUPID_BASE_BEGIN

bool get_system_ifconfig(std::vector<ifconfig_t> & ifconfigs)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        return false;
    }

    char buffer[1024] = { 0x0 };
    struct ifconf ifc = { 0x0 };
    ifc.ifc_buf = buffer;
    ifc.ifc_len = sizeof(buffer);
    if (0 != ioctl(sock, SIOCGIFCONF, &ifc))
    {
        close(sock);
        return false;
    }

    for (std::size_t index = 0, count = ifc.ifc_len / sizeof(struct ifreq); index < count; ++index)
    {
        ifconfig_t ifconfig = { 0x0 };

        struct ifreq ifr = { 0x0 };
        strncpy(ifr.ifr_name, ifc.ifc_req[index].ifr_name, sizeof(ifr.ifr_name) - 1);

        if (0 != ioctl(sock, SIOCGIFADDR, &ifr))
        {
            continue;
        }

        struct sockaddr_in * ip_addr = reinterpret_cast<struct sockaddr_in *>(&ifr.ifr_addr);
        inet_ntop(AF_INET, &ip_addr->sin_addr, ifconfig.ip, sizeof(ifconfig.ip) - 1);
        if (0 == strcmp("0.0.0.0", ifconfig.ip) || 0 == strcmp("127.0.0.1", ifconfig.ip))
        {
            continue;
        }

        if (0 != ioctl(sock, SIOCGIFHWADDR, &ifr))
        {
            continue;
        }

        unsigned char * mac = reinterpret_cast<unsigned char *>(ifr.ifr_hwaddr.sa_data);
        snprintf(ifconfig.mac, sizeof(ifconfig.mac), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        if (0 == strcmp("00:00:00:00:00:00", ifconfig.mac))
        {
            continue;
        }

        if (0 == ioctl(sock, SIOCGIFNETMASK, &ifr))
        {
            struct sockaddr_in * mask_addr = reinterpret_cast<struct sockaddr_in *>(&ifr.ifr_netmask);
            inet_ntop(AF_INET, &mask_addr->sin_addr, ifconfig.mask, sizeof(ifconfig.mask) - 1);
        }

        if (0 == ioctl(sock, SIOCGIFBRDADDR, &ifr))
        {
            struct sockaddr_in * broadcast_addr = reinterpret_cast<struct sockaddr_in *>(&ifr.ifr_broadaddr);
            inet_ntop(AF_INET, &broadcast_addr->sin_addr, ifconfig.broadcast, sizeof(ifconfig.broadcast) - 1);
        }

        strncpy(ifconfig.name, ifr.ifr_name, sizeof(ifconfig.name) - 1);

        ifconfigs.push_back(ifconfig);
    }

    close(sock);

    return !ifconfigs.empty();
}

bool get_system_memory_usage(uint64_t & total_size, uint64_t & avali_size)
{
    std::ifstream ifs("/proc/meminfo", std::ios::binary);
    if (!ifs.is_open())
    {
        total_size = 1;
        avali_size = 0;
        return false;
    }

    total_size = 0;
    avali_size = 0;

    int got_count = 0;
    while (!ifs.eof())
    {
        char buffer[4096] = { 0x00 };
        ifs.getline(buffer, sizeof(buffer));
        mem_occupy_t mem_occupy;
        memset(&mem_occupy, 0x00, sizeof(mem_occupy));
        sscanf(buffer, "%s %llu %s", mem_occupy.name, &mem_occupy.size, mem_occupy.unit);
        if (0 == strncasecmp(mem_occupy.name, "MemTotal:", 9))
        {
            total_size = mem_occupy.size * memory_unit_bytes(mem_occupy.unit);
        }
        else if (0 == strncasecmp(mem_occupy.name, "MemFree:", 8))
        {
            avali_size += mem_occupy.size * memory_unit_bytes(mem_occupy.unit);
        }
        else if (0 == strncasecmp(mem_occupy.name, "Buffers:", 8))
        {
            avali_size += mem_occupy.size * memory_unit_bytes(mem_occupy.unit);
        }
        else if (0 == strncasecmp(mem_occupy.name, "Cached:", 7))
        {
            avali_size += mem_occupy.size * memory_unit_bytes(mem_occupy.unit);
        }
        else
        {
            continue;
        }
        if (4 == ++got_count)
        {
            break;
        }
    }
    ifs.close();

    if (0 == total_size)
    {
        total_size = 1;
    }
    if (total_size < avali_size)
    {
        total_size = avali_size;
    }

    return 4 == got_count;
}

bool get_system_disk_usage(const char * disk_path, uint64_t & total_size, uint64_t & avali_size)
{
    if (nullptr == disk_path)
    {
        total_size = 1;
        avali_size = 0;
        return false;
    }

    struct statvfs file_info;
    if (0 == statvfs(utf8_to_ansi(disk_path).c_str(), &file_info))
    {
        total_size = file_info.f_bsize * file_info.f_blocks;
        avali_size = file_info.f_bsize * file_info.f_bavail;
        return true;
    }
    else
    {
        total_size = 1;
        avali_size = 0;
        return false;
    }
}

bool get_system_cpu_usage(std::vector<size_t> & cpu_usage)
{
    cpu_usage.clear();

    const size_t max_core_count = 256;
    static size_t core_count = max_core_count;
    static bool first_time = true;
    static cpu_occupy_t old_occupy_array[max_core_count];
    cpu_occupy_t new_occupy_array[max_core_count];

    while (true)
    {
        std::ifstream ifs("/proc/stat", std::ios::binary);
        if (!ifs.is_open())
        {
            return false;
        }
        size_t index = 0;
        char buffer[4096] = { 0x00 };
        while (!ifs.eof() && index < max_core_count)
        {
            memset(buffer, 0x00, sizeof(buffer));
            ifs.getline(buffer, sizeof(buffer));
            cpu_occupy_t cpu_occupy;
            memset(&cpu_occupy, 0x00, sizeof(cpu_occupy));
            sscanf(buffer, "%s %u %u %u %u", cpu_occupy.name, &cpu_occupy.user, &cpu_occupy.nice, &cpu_occupy.system, &cpu_occupy.idle);
            if (0 != strncasecmp(cpu_occupy.name, "cpu", 3))
            {
                break;
            }
            memcpy(&new_occupy_array[index], &cpu_occupy, sizeof(cpu_occupy));
            ++index;
        }
        ifs.close();

        if (core_count > index)
        {
            core_count = index;
        }

        if (first_time)
        {
            first_time = false;
            memcpy(old_occupy_array, new_occupy_array, sizeof(old_occupy_array));
            sleep(1);
        }
        else
        {
            break;
        }
    }

    size_t query_core_index = (core_count > 1 ? 1 : 0);
    size_t query_core_count = core_count;

    for (size_t index = query_core_index; index < query_core_count; ++index)
    {
        const cpu_occupy_t & new_occupy = new_occupy_array[index];
        const cpu_occupy_t & old_occupy = old_occupy_array[index];
        unsigned long new_total = static_cast<unsigned long>(new_occupy.user + new_occupy.nice + new_occupy.system + new_occupy.idle);
        unsigned long old_total = static_cast<unsigned long>(old_occupy.user + old_occupy.nice + old_occupy.system + old_occupy.idle);
        unsigned long user_delta = static_cast<unsigned long>(new_occupy.user - old_occupy.user);
        unsigned long system_delta = static_cast<unsigned long>(new_occupy.system - old_occupy.system);
        if (new_total != old_total)
        {
            cpu_usage.push_back(static_cast<size_t>((user_delta + system_delta) * 100 / (new_total - old_total)));
        }
        else
        {
            cpu_usage.push_back(0);
        }
    }

    memcpy(old_occupy_array, new_occupy_array, sizeof(old_occupy_array));

    return true;
}

bool get_system_cpu_core_count(size_t & core_count)
{
    errno = 0;

    long processor_count = sysconf(_SC_NPROCESSORS_ONLN);

    if (0 != errno)
    {
        errno = 0;
        core_count = 1;
        return false;
    }

    core_count = static_cast<size_t>(processor_count);

    return true;
}

bool get_system_uptime(uint64_t & uptime)
{
    struct sysinfo sys_info = { 0x00 };
    if (0 != sysinfo(&sys_info))
    {
        uptime = 0;
        return false;
    }
    else
    {
        uptime = static_cast<uint64_t>(sys_info.uptime);
        return true;
    }
}

NAMESPACE_STUPID_BASE_END

#endif // _MSC_VER
