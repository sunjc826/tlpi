#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "tlpi_hdr.h"
#include <sys/utsname.h>

int main(int argc, char const *argv[])
{
    utsname uts;
    if (uname(&uts) == -1)
        err_exit("uname");

    std::printf("Node name: %s\n", uts.nodename);
    std::printf("System name: %s\n", uts.sysname);
    std::printf("Release: %s\n", uts.release);
    std::printf("Version: %s\n", uts.version);
    std::printf("Machine: %s\n", uts.machine);
#ifdef _GNU_SOURCE
    std::printf("Domain name: %s\n", uts.domainname);
#endif
    return 0;
}

