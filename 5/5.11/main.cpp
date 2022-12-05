#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int dup_stdout_v1()
{
    return dup(STDOUT_FILENO);
}

int dup_stdout_v2()
{
    return open("/dev/fd/1", O_WRONLY);
}

int dup_stdout_v3()
{
    return open("/dev/stdout", O_WRONLY);
}

