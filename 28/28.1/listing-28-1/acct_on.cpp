#define _BSD_SOURCE
#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    if (argc > 2 || (argc > 1 && std::strcmp(argv[1], "--help") == 0))
        usage_err("%s [file]\n", argv[0]);

    if (acct(argv[1]) == -1)
        err_exit("acct");

    std::printf("Process accounting %s\n", (argv[1] == nullptr) ? "disabled" : "enabled");
    return 0;
}
