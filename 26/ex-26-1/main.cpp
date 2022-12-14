#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block_mask, nullptr);
    std::setbuf(stdout, nullptr);
    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        std::printf("Parent pid: %ld\n", static_cast<long>(getppid()));
        if (kill(getppid(), SIGUSR1) == -1)
            err_exit_("kill");
        sleep(5);
        std::printf("Parent pid: %ld\n", static_cast<long>(getppid()));
        break;
    default:
        sigset_t empty_mask;
        sigemptyset(&empty_mask);
        if (sigsuspend(&empty_mask) == -1 && errno != SIGINT)
            err_exit("sigsuspend");
        exit(EXIT_SUCCESS);
        break;
    }

    return 0;
}
