#include "tlpi_hdr.h"


void sigint_handler(int sig)
{
    std::printf("Received signal: %d (%s)\n", sig, strsignal(sig)); /* UNSAFE: not async signal safe */
}

int main(int argc, char const *argv[])
{
    sigset_t sigint_mask;
    sigemptyset(&sigint_mask);
    sigaddset(&sigint_mask, SIGINT);

    // Make sure that SIGINT is not blocked
    sigprocmask(SIG_UNBLOCK, &sigint_mask, nullptr);

    sigaction_st sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);

    raise(SIGINT);


    sigprocmask(SIG_BLOCK, &sigint_mask, nullptr);

    print_pending_sigs(stdout, "Before raise");
    raise(SIGINT);
    print_pending_sigs(stdout, "After raise");

    print_pending_sigs(stdout, "Before ign");
    signal(SIGINT, SIG_IGN);
    print_pending_sigs(stdout, "After ign");

    sigprocmask(SIG_UNBLOCK, &sigint_mask, nullptr);

    return 0;
}
