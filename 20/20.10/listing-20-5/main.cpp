#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{

    sigset_t block_set, prev_mask;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);

    // block SIGINT
    sigprocmask(SIG_BLOCK, &block_set, &prev_mask);


    // restore previous signal mask
    sigprocmask(SIG_SETMASK, &prev_mask, nullptr);

    return 0;
}
