#include "tlpi_hdr.h"
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    if (argc > 1)
    {
        int flags{ fcntl(STDOUT_FILENO, F_GETFD) };
        if (flags == -1)
            err_exit("fcntl - F_GETFD");

        flags |= FD_CLOEXEC;

        if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1)
            err_exit("fcntl - F_SETFD");
    }

    execlp("ls", "ls", "-l", argv[0], static_cast<char *>(nullptr));
    err_exit("execlp");

    return 0;
}

