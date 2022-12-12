#include "tlpi_hdr.h"

int my_siginterrupt(int sig, int flag)
{
    sigaction_st current_sa;
    if (sigaction(sig, nullptr, &current_sa) == -1)
        return -1;
    if (sig == 0)
        current_sa.sa_flags |= SA_RESTART;
    else
        current_sa.sa_flags &= ~SA_RESTART;
    return sigaction(sig, &current_sa, nullptr);
}
