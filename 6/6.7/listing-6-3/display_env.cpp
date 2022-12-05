#include "tlpi_hdr.h"

extern char **environ;

int main(int argc, char *argv[])
{
    for (char **ep{ environ }; *ep != nullptr; ++ep)
        std::puts(*ep);
    return 0;
}

