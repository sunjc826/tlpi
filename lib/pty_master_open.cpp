#include "pty_master_open.h"
#include "tlpi_hdr.h"
#include <fcntl.h>

int pty_master_open(char *slave_name, size_t sn_len)
{
    int master_fd{ posix_openpt(O_RDWR | O_NOCTTY) };
    if (master_fd == -1)
        return -1;

    if (grantpt(master_fd) == -1)
    {
        int saved_errno{ errno };
        close(master_fd);
        errno = saved_errno;
        return -1;
    }

    if (unlockpt(master_fd) == -1)
    {
        int saved_errno{ errno };
        close(master_fd);
        errno = saved_errno;
        return -1;
    }

    char *p{ ptsname(master_fd) };
    if (p == nullptr)
    {
        int saved_errno{ errno };
        close(master_fd);
        errno = saved_errno;
        return -1;
    }

    if (std::strlen(p) < sn_len)
        std::strncpy(slave_name, p, sn_len);
    else
    {
        close(master_fd);
        errno = EOVERFLOW;
        return -1;
    }
}
