#include "tlpi_hdr.h"
#include <signal.h>

int main(int argc, char const *argv[])
{
    if (argc != 3 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s sig-num pid\n", argv[0]);

    int sig{ get_int(argv[1], 0, "sig-num") };
    int s{ kill(get_long(argv[2], 0, "pid"), sig) };

    if (sig != 0)
    {
        if (s == -1)
            err_exit("kill");
    }
    else
    {
        if (s == 0)
            std::printf("Process exists, but we dont have "
                "permission to send it a signal\n");
        else if (errno == ESRCH)
            std::printf("Process does not exist\n");
        else
            err_exit("kill");
    }

    return 0;
}
