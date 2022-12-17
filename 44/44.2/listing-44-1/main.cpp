#include "tlpi_hdr.h"

constexpr int read_idx{ 0 }, write_idx{ 1 };


int main(int argc, char const *argv[])
{
    int filedes[2];
    if (pipe(filedes) == -1)
        err_exit("pipe");

    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        if (close(filedes[write_idx]) == -1)
            err_exit("close");
        break;
    default:
        if (close(filedes[read_idx]) == -1)
            err_exit("close");
        break;
    }


    return 0;
}

