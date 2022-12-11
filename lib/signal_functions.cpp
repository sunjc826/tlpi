#include "signal_functions.h"
#include <cstring>
void print_sigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
    int cnt{ 0 };
    for (int sig{ 1 }; sig < NSIG; ++sig)
    {
        if (sigismember(sigset, sig))
        {
            ++cnt;
            std::fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }

    if (cnt == 0)
        std::fprintf(of, "%s<empty signal set>\n", prefix);
}

int print_sig_mask(FILE *of, const char *msg)
{
    sigset_t current_mask;
    if (msg != nullptr)
        std::fprintf(of, "%s", msg);

    if (sigprocmask(SIG_BLOCK, nullptr, &current_mask) == -1)
        return -1;

    print_sigset(of, "\t\t", &current_mask);
    return 0;
}

int print_pending_sigs(FILE *of, const char *msg)
{
    sigset_t pending_sigs;
    if (msg != nullptr)
        std::fprintf(of, "%s", msg);

    if (sigpending(&pending_sigs) == -1)
        return -1;

    print_sigset(of, "\t\t", &pending_sigs);
    return 0;
}
