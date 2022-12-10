#include "tlpi_hdr.h"
#include <signal.h>

void new_handler(int)
{
    std::puts("new_handler called");
}

int main(int argc, char *argv[])
{
    sighandler_t old_handler{ signal(SIGINT, new_handler) };
    if (old_handler == SIG_ERR)
        err_exit("signal");

    // Do something
    for (int i{ 0 }; i < 5; i++)
    {
        std::printf("%d\n", i);
        sleep(1);
    }

    if (signal(SIGINT, old_handler) == SIG_ERR)
        err_exit("signal");

    return 0;
}

