#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "tlpi_hdr.h"
#include <cassert>

extern char **environ;
int main(int argc, char *argv[])
{
    clearenv();
    assert(environ == nullptr);

    for (int i{ 1 }; i < argc; ++i)
        if (putenv(argv[i]) != 0)
            err_exit("putenv: %s", argv[i]);

    if (setenv("GREET", "Hello", static_cast<int>(false)))
        err_exit("setenv");

    unsetenv("BYE");

    for (char **ep{ environ }; *ep != nullptr; ++ep)
        std::puts(*ep);

    return 0;
}

