#define _DEFAULT_SOURCE
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc > 1) // any argument will suffice
        setlinebuf(stdout);

    std::printf("Hello world\n");
    write(STDOUT_FILENO, "Ciao\n", 5);
    if (fork() == -1)
        err_exit("fork");
    return 0;
}

