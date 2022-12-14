#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    std::printf("Initial value of USER: %s\n", std::getenv("USER"));
    if (putenv("USER=britta") != 0)
        err_exit("putenv");

    execl("/usr/bin/printenv", "printenv", "USER", "SHELL", static_cast<char *>(nullptr));
    err_exit("execl");

    return 0;
}
