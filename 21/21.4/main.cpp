#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    int retval{ TEMP_FAILURE_RETRY(std::printf("Hello world")) };
    return 0;
}
