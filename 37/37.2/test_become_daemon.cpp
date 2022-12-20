#include "become_daemon.h"
#include "tlpi_hdr.h"
int main(int argc, char *argv[])
{
    if (become_daemon(0) == -1)
        err_exit("become_daemon");
    for (int i{}; i < 10; ++i)
        sleep(5);
    return 0;
}

