#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int tty_fd{ open("/dev/tty", O_RDONLY) };
    if (tty_fd == -1)
        err_exit("open");

    pid_t foreground_pgid{ tcgetpgrp(tty_fd) };
    if (foreground_pgid == -1)
        err_exit("tcgetpgrp");

    std::printf("%ld\n", static_cast<long>(foreground_pgid));

    return 0;
}
