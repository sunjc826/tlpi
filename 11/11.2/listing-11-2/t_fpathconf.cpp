#include "tlpi_hdr.h"

static void fpathconf_print(const char *msg, int fd, int name)
{
    errno = 0;
    long limit{ fpathconf(fd, name) };
    if (limit != -1)
        std::printf("%s %ld\n", msg, limit);
    else
    {
        if (errno == 0)
            std::printf("%s (indeterminate)\n", msg);
        else
            err_exit("fpathconf %s", msg);
    }
}

int main(int argc, char const *argv[])
{
    fpathconf_print("_PC_NAME_MAX:", STDIN_FILENO, _PC_NAME_MAX);
    fpathconf_print("_PC_PATH_MAX:", STDIN_FILENO, _PC_PATH_MAX);
    fpathconf_print("_PC_PIPE_BUF:", STDIN_FILENO, _PC_PIPE_BUF);
    return 0;
}
