#define _XOPEN_SOURCE 500
#include "tlpi_hdr.h"

static void handler(int)
{
    std::printf("%ld: caught SIGHUP 1\n", static_cast<long>(getpid()));
}

int main(int argc, char *argv[])
{
    pid_t child_pid;
    sigaction_st sa;

    setbuf(stdout, nullptr);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGHUP, &sa, nullptr) == -1)
        err_exit("sigaction");

    child_pid = fork();

    if (child_pid == 0 && argc > 1)
    {
        if (setpgid(0, 0) == -1) // move to new process group
            err_exit("setpgid");
    }

    std::printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n",
        static_cast<long>(getpid()),
        static_cast<long>(getppid()),
        static_cast<long>(getpgrp()),
        static_cast<long>(getsid(0)));

    alarm(60);

    while (true)
    {
        pause();
        std::printf("%ld: caught SIGHUP 2\n", static_cast<long>(getpid()));
    }

    return 0;
}

