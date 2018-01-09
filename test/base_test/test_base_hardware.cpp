#include "test_base.h"
#include "base/filesystem/hardware.h"

USING_NAMESPACE_STUPID_BASE

void test_base_hardware(void)
{
    {
        uint64_t total_size = 0;
        uint64_t avali_size = 0;
        if (get_system_memory_usage(total_size, avali_size))
        {
            printf("memory : total - " STUPID_U64_FMT ", avali - " STUPID_U64_FMT "\n", total_size, avali_size);
        }
        else
        {
            printf("get system memory usage failed\n");
        }
    }

    {
#ifdef _MSC_VER
        const char * disk_path = "c:";
#else
        const char * disk_path = "/";
#endif // _MSC_VER
        uint64_t total_size = 0;
        uint64_t avali_size = 0;
        if (get_system_disk_usage(disk_path, total_size, avali_size))
        {
            printf("disk (%s) : total - " STUPID_U64_FMT ", avali - " STUPID_U64_FMT "\n", disk_path, total_size, avali_size);
        }
        else
        {
            printf("get system disk usage failed\n");
        }
    }

    {
        std::vector<size_t> cpu_usage;
        if (get_system_cpu_usage(cpu_usage))
        {
            printf("cpu : ");
            for (std::vector<size_t>::iterator iter = cpu_usage.begin(); cpu_usage.end() != iter; ++iter)
            {
                printf("%lu ", *iter);
            }
            printf("\n");
        }
        else
        {
            printf("get system cpu usage failed\n");
        }
    }

    {
        size_t cpu_core_count = 0;
        if (get_system_cpu_core_count(cpu_core_count))
        {
            printf("cpu core count : %u\n", cpu_core_count);
        }
        else
        {
            printf("get system cpu core count failed\n");
        }
    }

    {
        uint64_t uptime = 0;
        if (get_system_uptime(uptime))
        {
            printf("system uptime : " STUPID_U64_FMT "\n", uptime);
        }
        else
        {
            printf("get system uptime failed\n");
        }
    }
}
