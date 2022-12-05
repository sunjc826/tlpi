#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <cassert>
int main(int argc, char const *argv[])
{
    int fd{ open("test.txt", O_CREAT | O_TRUNC | O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) };
    if (fd == -1)
        err_exit("open");
    int dupfd{ dup(fd) };
    if (dupfd == -1)
        err_exit("dup");

    int fd_fl{ fcntl(fd, F_GETFL) };
    if (fd_fl == -1)
        err_exit("fcntl");
    int dupfd_fl{ fcntl(dupfd, F_GETFL) };
    if (dupfd_fl == -1)
        err_exit("fcntl");
    assert(fd_fl == dupfd_fl);

    if (lseek(fd, 10, SEEK_END) == -1)
        err_exit("lseek");

    off_t fd_offset{ lseek(fd, 0, SEEK_CUR) };
    if (fd_offset == -1)
        err_exit("fcntl");
    off_t dupfd_offset{ lseek(dupfd, 0, SEEK_CUR) };
    if (dupfd_offset == -1)
        err_exit("fcntl");
    assert(fd_offset == dupfd_offset);

    return 0;
}

