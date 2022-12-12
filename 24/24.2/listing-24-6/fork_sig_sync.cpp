#include "tlpi_hdr.h"
#include "curr_time.h"
#include <signal.h>

constexpr int sync_sig{ SIGUSR1 };

static void handler(int)
{
}

int main(int argc, char *argv[])
{
    sigset_t block_mask, original_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, sync_sig);
    if (sigprocmask(SIG_BLOCK, &block_mask, &original_mask) == -1)
        err_exit("sigprocmask");

    sigaction_st sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(sync_sig, &sa, nullptr) == -1)
        err_exit("sigaction");

    int child_pid;
    switch (child_pid = fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        /* Child does some action before parent */

        std::printf("[%s %ld] Child started - doing some work\n",
            curr_time("%T"), static_cast<long>(getpid()));
        sleep(2);
        std::printf("[%s %ld] Child about to signal parent\n",
            curr_time("%T"), static_cast<long>(getpid()));
        if (kill(getppid(), sync_sig) == -1)
            err_exit("kill");

        /* Child does other things */

        _exit(EXIT_SUCCESS);
        break;
    default:
        /* Parent does some things before waiting for child to complete the required action */

        std::printf("[%s %ld] Parent about to wait for signal\n",
            curr_time("%T"), static_cast<long>(getpid()));
        sigset_t empty_mask;
        sigemptyset(&empty_mask);
        if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
            err_exit("sigsuspend");
        std::printf("[%s %ld] Parent got signal\n", curr_time("%T"), static_cast<long>(getpid()));

        /* if required, return signal mask back to original state */

        if (sigprocmask(SIG_SETMASK, &original_mask, nullptr) == -1)
            err_exit("sigprocmask");

        /* Parent carries on to do other things */
        break;
    }

    return 0;
}

