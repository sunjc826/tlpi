#define _GNU_SOURCE
#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

static int child_func(void *arg)
{
    if (close(*static_cast<int *>(arg)) == -1)
        err_exit("close");

    return 0;
}

int main(int argc, char *argv[])
{
    int fd{ open("/dev/null", O_RDWR) };
    if (fd == -1)
        err_exit("open");


    int flags{ (argc > 1) ? CLONE_FILES : 0 };
    constexpr int stack_size{ 65536 };
    char *stack{ static_cast<char *>(std::malloc(stack_size)) };
    if (stack == nullptr)
        err_exit("malloc");
    char *stack_top{ stack + stack_size }; // assume stack grows downward

    if (CHILD_SIG != 0 && CHILD_SIG != SIGCHLD)
    {
        if (signal(CHILD_SIG, SIG_IGN) == SIG_ERR)
            err_exit("signal");
    }

    if (clone(child_func, stack_top, flags | CHILD_SIG, &fd) == -1)
        err_exit("clone");

    if (waitpid(-1, nullptr, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0) == -1)
        err_exit("waitpid");
    std::puts("child has terminated");

    ssize_t s{ write(fd, "x", 1) };
    if (s == -1 && errno == EBADF)
        std::printf("file descriptor %d has been closed\n", fd);
    else if (s == -1)
        std::printf("write() on file descriptor %d failed unexpectedly (%s)\n", fd, strerror(errno));
    else
        std::printf("write() on file descriptor %d succeeded\n", fd);

    return 0;
}

