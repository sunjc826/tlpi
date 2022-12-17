#include "tlpi_hdr.h"
#include <sys/wait.h>

constexpr size_t buf_size{ 10lu };

int main(int argc, char *argv[])
{
    int pfd[2];
    char buf[buf_size];
    ssize_t num_read;

    if (argc != 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s string\n", argv[0]);

    if (pipe(pfd) == -1)
        err_exit("pipe");

    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        if (close(pfd[1]) == -1)
            err_exit_("close - child");

        while (true)
        {
            ssize_t num_read{ read(pfd[0], buf, buf_size) };
            if (num_read == -1)
                err_exit("read");
            if (num_read == 0)
                break;
            if (write(STDOUT_FILENO, buf, num_read) != num_read)
                fatal("child - partial/failed write");
        }

        write(STDOUT_FILENO, "\n", 1);
        if (close(pfd[0]) == -1)
            err_exit_("close");

        _exit(EXIT_SUCCESS);
    default:
        if (close(pfd[0]) == -1)
            err_exit("close - parent");
        if (write(pfd[1], argv[1], std::strlen(argv[1])) != std::strlen(argv[1]))
            fatal("parent - partial/failed write");
        if (close(pfd[1]) == -1)
            err_exit("close");
        wait(nullptr);
        break;
    }

    return 0;
}

