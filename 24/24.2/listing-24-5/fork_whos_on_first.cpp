#include "tlpi_hdr.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usage_err("%s [num-children]\n", argv[0]);

    int num_children{ (argc > 1) ? get_int(argv[1], GN_GT_0, "num-children") : 1 };
    std::setbuf(stdout, nullptr);

    for (int i{ 0 }; i < num_children; ++i)
    {
        int child_pid;
        switch (child_pid = fork())
        {
        case -1:
            err_exit("fork");
            break;
        case 0:
            std::printf("%d child\n", i);
            _exit(EXIT_SUCCESS);
            break;
        default:
            std::printf("%d parent\n", i);
            wait(nullptr);
            break;
        }
    }

    return 0;
}

