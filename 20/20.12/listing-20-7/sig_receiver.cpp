#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "tlpi_hdr.h"

static int sig_cnt[NSIG];
static volatile sig_atomic_t got_sigint{ 0 };

static void handler(int sig)
{
    if (sig == SIGINT)
        got_sigint = 1;
    else
        ++sig_cnt[sig];
}

int main(int argc, char const *argv[])
{
    std::printf("%s: PID is %ld\n", argv[0], static_cast<long>(getpid()));

    for (int n{ 1 }; n < NSIG; ++n)
        signal(n, handler);

    if (argc > 1)
    {
        int num_secs{ get_int(argv[1], GN_GT_0, nullptr) };
        sigset_t blocking_mask;
        sigfillset(&blocking_mask);
        if (sigprocmask(SIG_SETMASK, &blocking_mask, nullptr) == -1)
            err_exit("sigprocmask");

        std::printf("%s: sleeping for %d seconds\n", argv[0], num_secs);

        sleep(num_secs);

        sigset_t pending_mask;
        if (sigpending(&pending_mask) == -1)
            err_exit("sigpending");

        std::printf("%s: pending signals are: \n", argv[0]);
        print_sigset(stdout, "\t\t", &pending_mask);

        sigset_t empty_mask;
        sigemptyset(&empty_mask);
        if (sigprocmask(SIG_SETMASK, &empty_mask, nullptr) == -1)
            err_exit("sigprocmask");

    }

    while (!got_sigint)
        continue;

    for (int n{ 1 }; n < NSIG; ++n)
        if (sig_cnt[n] != 0)
            std::printf("%s: signal %d caught %d time%s\n",
                argv[0], n, sig_cnt[n], (sig_cnt[n] == 1) ? "" : "s");

    return 0;
}

