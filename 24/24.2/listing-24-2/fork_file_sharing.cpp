#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    char template_str[] = "/tmp/testXXXXXX";

    setbuf(stdout, nullptr);

    int fd{ mkstemp(template_str) };
    if (fd == -1)
        err_exit("mkstemp");

    std::printf("File offset before fork(): %jd\n",
        static_cast<intmax_t>(lseek(fd, 0, SEEK_CUR)));

    int flags{ fcntl(fd, F_GETFL) };
    if (flags == -1)
        err_exit("fcntl - F_GETFL");
    std::printf("O_APPEND flag before fork() is: %s\n",
        (flags & O_APPEND) ? "on" : "off");

    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        if (lseek(fd, 1000, SEEK_SET) == -1)
            err_exit("lseek");
        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
            err_exit("fcntl - F_GETFL");
        flags |= O_APPEND;
        if (fcntl(fd, F_SETFL, flags) == -1)
            err_exit("fcntl - F_SETFL");
        _exit(EXIT_SUCCESS);
        break;
    default:
        if (wait(nullptr) == -1)
            err_exit("wait");
        std::puts("Child has exited");
        break;
    }

    std::printf("File offset in parent: %jd\n", static_cast<intmax_t>(lseek(fd, 0, SEEK_CUR)));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        err_exit("fcntl - F_GETFL");

    std::printf("O_APPEND flag in parent is: %s\n",
        (flags & O_APPEND) ? "on" : "off");

    return 0;
}

