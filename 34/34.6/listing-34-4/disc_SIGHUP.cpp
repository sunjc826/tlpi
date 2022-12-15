#define _GNU_SOURCE
#include "tlpi_hdr.h"

static void handler(int sig)
{
    std::printf("PID %ld: caught signal %2d (%s)\n",
        static_cast<long>(getpid()),
        sig,
        strsignal(sig)); /* UNSAFE */
}

int main(int argc, char *argv[])
{

    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s {d|s}... [ > sig.log 2>&1]\n", argv[0]);

    std::setbuf(stdout, nullptr);

    pid_t parent_pid{ getpid() };

    std::printf("PID of parent process is:       %ld\n", static_cast<long>(parent_pid));
    std::printf("Foreground process group ID is: %ld\n", static_cast<long>(tcgetpgrp(STDIN_FILENO)));

    for (int i{ 1 }; i < argc; ++i)
    {
        pid_t child_pid{ fork() };
        if (child_pid == -1)
            err_exit("fork");

        if (child_pid == 0)
        {
            if (argv[i][0] == 'd')
            {
                if (setpgid(0, 0) == -1)
                    err_exit("setpgid");
            }
            sigaction_st sa;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SIGHUP, &sa, nullptr) == -1)
                err_exit("sigaction");
            break;
        }
    }

    alarm(60);

    std::printf("PID=%ld PGID=%ld\n",
        static_cast<long>(getpid()),
        static_cast<long>(getpgrp()));

    while (true)
        pause();

    return 0;
}

