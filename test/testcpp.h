#pragma once 
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string>
#include <stdbool.h>
#include <string.h>
#include <memory>
#include <iostream>
namespace base{
    void get_gcc_version(const char* os_buf, char* os_string) {
        if(nullptr == os_buf) {
            return ;
        }
    char _target_gcc_version[7];
    snprintf(_target_gcc_version, 7, "10.2.0");
    char _gcc_version[7];
    snprintf(_gcc_version, 7, "%d.%d.%d",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
    printf("gcc version: %s\n", _gcc_version);
    assert(strcmp(_gcc_version, _target_gcc_version) == 0);
}

    /* get size of array. */
    int64_t get_sizeof_array(const char* string_buffer);

    /* test low-level const passing. */
    int64_t get_const_number(const char* string_buffer);
    
}
