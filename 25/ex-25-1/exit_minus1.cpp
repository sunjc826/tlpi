#include "tlpi_hdr.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        _exit(-1);
        break;
    default:
        int status;
        if (wait(&status) == -1)
            err_exit("wait");
        int exit_status{ WEXITSTATUS(status) };
        std::printf("%d\n", exit_status);
    }
    return 0;
}
