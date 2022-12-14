#include "tlpi_hdr.h"

extern char **environ;

int main(int argc, char const *argv[])
{
    for (int i{}; i < argc; ++i)
        std::printf("argv[%d] = %s\n", i, argv[i]);

    for (char **ep{ environ }; *ep != nullptr; ++ep)
        std::printf("environ: %s\n", *ep);

    return 0;
}

