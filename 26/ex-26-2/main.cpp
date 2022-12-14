#include "tlpi_hdr.h"
#include <sys/wait.h>

#define PRINT_PPID() (std::printf("Parent pid: %ld\n", static_cast<long>(getppid())))

pid_t grandparent_pid{ getpid() };
sigset_t empty_mask, block_mask;

static void do_child()
{
    PRINT_PPID();
    if (kill(getppid(), SIGUSR1) == -1)
        err_exit_("kill");

    sleep(3);
    PRINT_PPID(); // This should already point towards init process

    if (kill(grandparent_pid, SIGUSR2) == -1)
        err_exit_("kill");
    sleep(3);
    PRINT_PPID();

    _exit(EXIT_SUCCESS);
}

static void do_parent()
{
    if (sigprocmask(SIG_BLOCK, &block_mask, nullptr) == -1)
        err_exit_("sigprocmask");

    pid_t child_pid;
    switch (child_pid = fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        do_child();
        break;
    default:
        break;
    }
    if (sigsuspend(&empty_mask) == -1)
        err_exit_("sigsuspend");

    _exit(EXIT_SUCCESS);
}

void handler(int sig)
{
    psignal(sig, "handler received signal");
    if (wait(nullptr) == -1)
        err_exit_("wait");
}

int main(int argc, char *argv[])
{
    std::setbuf(stdout, nullptr);
    sigaction_st sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGUSR2, &sa, nullptr) == -1)
        err_exit("sigaction");

    if (grandparent_pid == -1)
        err_exit("getpid");
    sigemptyset(&empty_mask);
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);
    sigaddset(&block_mask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &block_mask, nullptr) == -1)
        err_exit("sigprocmask");

    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        do_parent();
        break;
    default:
        break;
    }

    if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
        err_exit("sigsuspend");

    return 0;
}
