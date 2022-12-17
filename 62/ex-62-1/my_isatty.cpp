#include "tlpi_hdr.h"
#include <termios.h>

bool my_isatty(int fd)
{
    termios term_attr;
    if (tcgetattr(fd, &term_attr) == -1)
    {
        err_msg("tcgetattr err: fd=%d", fd);
        errno = 0;
        return false;
    }

    return true;
}

int main(int argc, char const *argv[])
{
    int fd{ argc > 1 ? get_int(argv[1], GN_NONNEG, "fd") : 0 };

    std::puts(my_isatty(fd) ? "Is terminal" : "Not terminal");
    return 0;
}
