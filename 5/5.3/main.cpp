#include "tlpi_hdr.h"
#include <fcntl.h>

void set_append(int fd)
{
    int flags{ fcntl(fd, F_GETFL) };
    if (flags == -1)
        err_exit("fcntl");
    flags |= O_APPEND;
    if (fcntl(fd, F_SETFL, flags) == -1)
        err_exit("fcntl");
}

bool is_writable(int fd)
{
    int flags{ fcntl(fd, F_GETFL) };
    if (flags == -1)
        err_exit("fcntl");
    int access_mode{ flags & O_ACCMODE };
    return access_mode == O_WRONLY || access_mode == O_RDWR;
}
