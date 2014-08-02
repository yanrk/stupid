#include "test_base.h"
#include "base/filesystem/sys_io.h"
#include "base/config/ini.h"

USING_NAMESPACE_STUPID_BASE

const std::string app_name[] = 
{
    "", "jack", "lucy", "alice", "james"
};

const int app_size = sizeof(app_name) / sizeof(app_name[0]);

const std::string key_name[] = 
{
    "remark", "gender", "height", "weight", "year"
};

const int key_size = sizeof(key_name) / sizeof(key_name[0]);

const std::string value_1[app_size][key_size] = 
{
    { "who",      "?", "1.42cm", "45kg", "15" }, 
    {   "-",   "male", "1.44cm", "47kg", "15" }, 
    {   "-", "female", "1.36cm", "35kg", "15" }, 
    {   "-", "female", "1.40cm", "37kg", "15" }, 
    {   "-",   "male", "1.46cm", "50kg", "15" }
};

const std::string value_2[app_size][key_size] = 
{
    { "who",      "?", "1.65cm", "58kg", "18" }, 
    {   "-",   "male", "1.75cm", "65kg", "18" }, 
    {   "-", "female", "1.58cm", "45kg", "18" }, 
    {   "-", "female", "1.60cm", "46kg", "18" }, 
    {   "-",   "male", "1.73cm", "63kg", "18" }
};

void test_base_ini(void)
{
    const char * file_name = "./test.ini";

    Ini ini;
    if (!ini.load(file_name, ';', true, true))
    {
        printf("load %s failed\n", file_name);
        return;
    }

    for (int i = 0; i < app_size; ++i)
    {
        for (int j = 0; j < key_size; ++j)
        {
            const std::string & app = app_name[i];
            const std::string & key = key_name[j];
            std::string value;
            if (!ini.get_value(app, key, value))
            {
                printf("get_value(%s, %s) failed\n", app.c_str(), key.c_str());
            }
            const std::string & new_value1 = value_1[i][j];
            const std::string & new_value2 = value_2[i][j];
            if (new_value1 == value)
            {
                value = new_value2;
            }
            else
            {
                value = new_value1;
            }
            ini.set_value(app, key, value);
        }
    }

    if (!ini.save())
    {
        printf("save %s failed\n", file_name);
    }

    std::string app("alice");
    std::string key("year");
    int value = 0;
    if (!ini.get_value(app, key, value))
    {
        printf("get_value failed\n");
        return;
    }

    if (!ini.set_value(app, key, value-1))
    {
        printf("set_value failed\n");
    }
}
