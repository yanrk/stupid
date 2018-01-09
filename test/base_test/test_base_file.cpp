#include "test_base.h"
#include "base/filesystem/file.h"

USING_NAMESPACE_STUPID_BASE

static void test_base_file_1(void)
{
    const char * filename = "./test_file.txt";

    File file;
    if (!file.open(filename, true, true))
    {
        printf("open (%s) failed\n", filename);
        return;
    }

    const char data[] = "hello world\n";
    if (!file.write(data, sizeof(data) - 1))
    {
        printf("write file failed\n");
        return;
    }

    file.close();
}

static void test_base_file_2(void)
{
    const char * filename = "./test_file.txt";

    File file;
    if (!file.open(filename, true, false))
    {
        printf("open (%s) failed\n", filename);
        return;
    }

    const char data[] = "hello world\n";
    if (!file.write(data, sizeof(data) - 1))
    {
        printf("write file failed\n");
        return;
    }

    file.close();
}

static void test_base_file_3(void)
{
    const char * filename = "./test_file.txt";

    File file;
    if (!file.open(filename, false, false))
    {
        printf("open (%s) failed\n", filename);
        return;
    }

    char buff[64] = { 0x00 };
    size_t read_len = 0;
    if (!file.read(buff, sizeof(buff), read_len))
    {
        printf("read file failed\n");
        return;
    }

    if (0 != strcmp(buff, "hello world\nhello world\n"))
    {
        printf("read data exception\n");
        return;
    }

    if (!file.is_eof())
    {
        printf("file position should be eof\n");
        return;
    }

    file.close();
}

void test_base_file()
{
    test_base_file_1();
    test_base_file_2();
    test_base_file_3();
}
