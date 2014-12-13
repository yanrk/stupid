#include "test_base.h"

int main(int argc, char * argv[])
{
    test_base_string();
    test_base_hex();
    test_base_base64();
    test_base_exception();
    test_base_bytestream();
    test_base_memory();
    test_base_locker();
    test_base_guard();
    test_base_thread();
    test_base_object_pool();
    test_base_time();
    test_base_log();
    test_base_directory();
    test_base_ini();
    test_base_single_timer();
    test_base_multi_timer();
    test_base_id_manager();
    test_base_singleton();
    test_base_splitter();
    test_base_bcd();
    test_base_charset();
    test_base_counter();
    return(0);
}
