#include "tlpi_hdr.h"

static int idata{ 111 };
int main(int argc, char const *argv[])
{
    int istack{ 222 };
    pid_t child_pid;
    switch (child_pid = fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        idata *= 3;
        istack *= 3;
        break;
    default:
        sleep(3);
        break;
    }

    std::printf("PID=%ld %s idata=%d istack=%d\n",
        static_cast<long>(getpid()), (child_pid == 0) ? "(child)" : "(parent)", idata, istack);

    return 0;
}

