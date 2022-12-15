#define _XOPEN_SOURCE 500
#include "tlpi_hdr.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (fork() != 0)
        _exit(EXIT_SUCCESS);

    if (setsid() == -1)
        err_exit("setsid");

    std::printf("PID=%ld, PGID=%ld, SID=%ld\n",
        static_cast<long>(getpid()),
        static_cast<long>(getpgrp()),
        static_cast<long>(getsid(0)));

    if (open("/dev/tty", O_RDWR) == -1)
        err_exit("open /dev/tty");

    return 0;
}

