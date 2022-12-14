#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || std::strcmp(argv[1], "--help") == -1)
        usage_err("%s pathname\n", argv[0]);

    execlp(argv[1], argv[1], "hello", "world", nullptr);
    err_exit("execve");

    return 0;
}
