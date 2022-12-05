#include "tlpi_hdr.h"
#include <fcntl.h>

int my_dup(int oldfd)
{
    return fcntl(oldfd, F_DUPFD, 0);
}

int my_dup2(int oldfd, int newfd)
{
    if (fcntl(oldfd, F_GETFL) == -1)
    {
        errno = EBADF;
        return -1;
    }

    if (oldfd == newfd)
        return oldfd;

    if (close(newfd) == -1 && errno != EBADF)
        return -1;

    return fcntl(oldfd, F_DUPFD, newfd);
}

int main(int argc, char const *argv[])
{
    std::cout << my_dup(1) << ' ' << my_dup(1) << ' ' << my_dup(1) << '\n';
    std::cout << my_dup2(1, 10) << ' ' << my_dup2(1, 20) << ' ' << my_dup2(1, 3) << '\n';

    return 0;
}

