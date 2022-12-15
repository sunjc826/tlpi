#include "tlpi_hdr.h"

void tstp_handler(int sig)
{
    int saved_errno{ errno };

    std::puts("Caught SIGTSTP"); /* UNSAFE */

    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        err_exit("signal");

    raise(SIGTSTP);

    sigset_t tstp_mask, prev_mask;
    sigemptyset(&tstp_mask);
    sigaddset(&tstp_mask, SIGTSTP);
    if (sigprocmask(SIG_UNBLOCK, &tstp_mask, &prev_mask) == -1)
        err_exit_("sigprocmask");

    // Execution resumes after SIGCONT

    if (sigprocmask(SIG_SETMASK, &prev_mask, nullptr) == -1)
        err_exit_("sigprocmask");

    sigaction_st sa;
    sa.sa_handler = tstp_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGTSTP, &sa, nullptr) == -1)
        err_exit_("sigaction");

    std::puts("Exiting SIGTSTP handler"); /* UNSAFE */

    errno = saved_errno;
}

int main(int argc, char *argv[])
{
    sigaction_st sa;

    if (sigaction(SIGTSTP, nullptr, &sa) == -1)
        err_exit("sigaction");

    if (sa.sa_handler != SIG_IGN)
    {
        sa.sa_handler = tstp_handler;
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIGTSTP, &sa, nullptr) == -1)
            err_exit("sigaction");
    }

    while (true)
        pause();

    return 0;
}

