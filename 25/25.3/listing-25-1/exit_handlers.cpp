#define _DEFAULT_SOURCE
#include "tlpi_hdr.h"

static void atexit_func1(void)
{
    std::printf("atexit function 1 called\n");
}

static void atexit_func2(void)
{
    std::printf("atexit function 2 called\n");
}

static void on_exit_func(int exit_status, void *arg)
{
    std::printf("on_exit function called: status=%d, arg=%ld\n",
        exit_status, reinterpret_cast<long>(arg));
}

int main(int argc, char const *argv[])
{
    if (on_exit(on_exit_func, reinterpret_cast<void *>(10)) != 0)
        fatal("on_exit 1");

    if (atexit(atexit_func1) != 0)
        fatal("atexit 1");

    if (atexit(atexit_func2) != 0)
        fatal("atexit 2");

    if (on_exit(on_exit_func, reinterpret_cast<void *>(20)) != 0)
        fatal("on_exit 2");

    return 2;
}


