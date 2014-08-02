#include "test_base.h"
#include "base/log/log.h"
#include "base/utility/log_switch.h"
#include "base/utility/singleton.h"

USING_NAMESPACE_STUPID_BASE

static void test_log_with_write_mode_1(LOG_WRITE_MODE write_mode, const char * mode)
{
    LOG_CONFIG log_config;
    log_config.log_file_path = "./test_base_log/log_1/";
    if (nullptr != mode)
    {
        log_config.log_file_path += mode;
    }
    for (size_t type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
    {
        log_config.log_file[type].write_mode = write_mode;
        log_config.log_file[type].log_min_level = TRK_LEVEL;
        log_config.log_file[type].log_file_size = 10;
        log_config.log_file[type].buffer_count = 50;
        log_config.log_file[type].output_to_console = false;
    }

    log_init(log_config);

    for (size_t index = 0; index < 10; ++index)
    {
        RUN_LOG_CRI("%s %d", "test RUN_LOG_CRI", 1);
        RUN_LOG_ERR("%s %d", "test RUN_LOG_ERR", 2);
        RUN_LOG_WAR("%s %d", "test RUN_LOG_WAR", 3);
        RUN_LOG_DBG("%s %d", "test RUN_LOG_DBG", 4);
        RUN_LOG_TRK("%s %d", "test RUN_LOG_TRK", 5);
        DBG_LOG("%s %d", "test DBG_LOG", 6);
    }

    log_exit();
}

static void test_log_with_write_mode_2(LOG_WRITE_MODE write_mode, const char * mode)
{
    LOG_CONFIG log_config;
    log_config.log_file_path = "./test_base_log/log_2/";
    if (nullptr != mode)
    {
        log_config.log_file_path += mode;
    }
    for (size_t type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
    {
        log_config.log_file[type].write_mode = write_mode;
        log_config.log_file[type].log_min_level = TRK_LEVEL;
        log_config.log_file[type].log_file_size = 10;
        log_config.log_file[type].buffer_count = 50;
        log_config.log_file[type].output_to_console = false;
    }

    Singleton<LogSwitch>::instance().init(log_config);

    for (size_t index = 0; index < 10; ++index)
    {
        RUN_LOG_CRI("%s %d", "test RUN_LOG_CRI", 1);
        RUN_LOG_ERR("%s %d", "test RUN_LOG_ERR", 2);
        RUN_LOG_WAR("%s %d", "test RUN_LOG_WAR", 3);
        RUN_LOG_DBG("%s %d", "test RUN_LOG_DBG", 4);
        RUN_LOG_TRK("%s %d", "test RUN_LOG_TRK", 5);
        DBG_LOG("%s %d", "test DBG_LOG", 6);
    }

    Singleton<LogSwitch>::instance().exit();
}

static void test_log_with_write_mode_3(const char * config_file)
{
    Singleton<LogSwitch>::instance().init(config_file);

    for (size_t index = 0; index < 10; ++index)
    {
        RUN_LOG_CRI("%s %d", "test RUN_LOG_CRI", 1);
        RUN_LOG_ERR("%s %d", "test RUN_LOG_ERR", 2);
        RUN_LOG_WAR("%s %d", "test RUN_LOG_WAR", 3);
        RUN_LOG_DBG("%s %d", "test RUN_LOG_DBG", 4);
        RUN_LOG_TRK("%s %d", "test RUN_LOG_TRK", 5);
        DBG_LOG("%s %d", "test DBG_LOG", 6);
    }

    Singleton<LogSwitch>::instance().exit();
}

void test_base_log(void)
{
    {
        test_log_with_write_mode_1(SYNC_WRITE_MODE, "sync");
        test_log_with_write_mode_1(ASYN_WRITE_MODE, "asyn");
        test_log_with_write_mode_1(LAZY_WRITE_MODE, "lazy");
    }

    {
        test_log_with_write_mode_2(SYNC_WRITE_MODE, "sync");
        test_log_with_write_mode_2(ASYN_WRITE_MODE, "asyn");
        test_log_with_write_mode_2(LAZY_WRITE_MODE, "lazy");
    }

    {
        test_log_with_write_mode_3("./log.ini");
    }
}
