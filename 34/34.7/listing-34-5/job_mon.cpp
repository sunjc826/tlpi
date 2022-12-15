#include "tlpi_hdr.h"

static int cmd_num;

static void handler(int sig)
{
    /* UNSAFE: fprintf, strsignal */
    if (getpid() == getpgrp())
        std::fprintf(stderr, "Terminal FG process group: %ld\n", static_cast<long>(tcgetpgrp(STDERR_FILENO)));
    std::fprintf(stderr, "Process %ld (%d) received signal %d (%s)\n",
        static_cast<long>(getpid()), cmd_num, sig, strsignal(sig));

    if (sig == SIGTSTP)
        raise(SIGSTOP);
}

int main(int argc, char *argv[])
{

    sigaction_st sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, nullptr) == -1)
        err_exit("sigaction");
    if (sigaction(SIGTSTP, &sa, nullptr) == -1)
        err_exit("sigaction");
    if (sigaction(SIGCONT, &sa, nullptr) == -1)
        err_exit("sigaction");

    if (isatty(STDIN_FILENO))
    {
        std::fprintf(stderr, "Terminal FG process group: %ld\n", static_cast<long>(tcgetpgrp(STDIN_FILENO)));
        std::fprintf(stderr, "Command  PID  PPID  PGRP    SID\n");
        cmd_num = 0;
    }
    else
    {
        if (read(STDIN_FILENO, &cmd_num, sizeof(cmd_num)) <= 0)
            fatal("read got EOF or error");
    }

    ++cmd_num;
    std::fprintf(stderr, "%4d    %5ld %5ld %5ld %5ld\n",
        cmd_num, static_cast<long>(getpid()), static_cast<long>(getppid()), static_cast<long>(getpgrp()), static_cast<long>(getsid(0)));


    if (!isatty(STDOUT_FILENO))
    {
        if (write(STDOUT_FILENO, &cmd_num, sizeof(cmd_num)) == -1)
            err_msg("write");
    }

    while (true)
        pause();

    return 0;
}

