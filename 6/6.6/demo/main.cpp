#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <climits>
int main(int argc, char const *argv[])
{
    std::printf("program_invocation_name: %s\n", program_invocation_name);
    std::printf("program_invocation_short_name: %s\n", program_invocation_short_name);
    int fd{ open("/proc/self/cmdline", O_RDONLY) };
    char buf[1000];
    ssize_t num_read{ read(fd, buf, 1000) };
    buf[num_read] = '\0';
    std::printf("/proc/self/cmdline: %s\n", buf);

    std::printf("ARG_MAX: %jd\n", static_cast<std::intmax_t>(sysconf(_SC_ARG_MAX)));

    return 0;
}

