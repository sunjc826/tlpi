#include "tlpi_hdr.h"
#include <signal.h>

static void sig_handler(int sig)
{
    static int count{ 0 };

    if (sig == SIGINT)
    {
        ++count;
        std::printf("Caught SIGINT (%d)\n", count);
        return;
    }

    std::printf("Caught SIGQUIT - that's all folks!\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        err_exit("signal");
    if (signal(SIGQUIT, sig_handler) == SIG_ERR)
        err_exit("signal");
    while (true)
        pause();
    return 0;
}

