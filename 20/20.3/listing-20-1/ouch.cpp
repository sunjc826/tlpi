#include "tlpi_hdr.h"
#include <signal.h>

static void sig_handler(int sig)
{
    std::printf("Ouch\n");
}

int main(int argc, char *argv[])
{
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        err_exit("signal");

    for (int i{ 0 }; ; ++i)
    {
        std::printf("%d\n", i);
        sleep(3);
    }
    return 0;
}

