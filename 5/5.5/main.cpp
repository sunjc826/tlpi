#include "tlpi_hdr.h"
#include <fcntl.h>

int dup_set_cloexec_v1(int oldfd, int newfd)
{
    int val{ dup3(oldfd, newfd, O_CLOEXEC) };
    if (val == -1)
        err_exit("dup3");
    return val;
}

int dup_set_cloexec_v2(int oldfd, int newfd)
{
    if (close(newfd) == -1)
        if (errno != EBADF)
            err_exit("close");
    int val{ fcntl(oldfd, F_DUPFD_CLOEXEC, newfd) };
    if (val == -1)
        err_exit("fcntl");
    return val;
}
