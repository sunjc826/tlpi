#include "tlpi_hdr.h"

sighandler_t my_signal(int sig, sighandler_t handler)
{
    sigaction_st sa, prev_sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags =
#ifdef OLD_SIGNAL
        SA_RESETHAND | SA_NODEFER
#else
        SA_RESTART
#endif
        ;
    if (sigaction(sig, &sa, &prev_sa) == -1)
        return SIG_ERR;
    else
        return prev_sa.sa_handler;
}
