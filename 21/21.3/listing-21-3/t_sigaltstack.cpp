#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "tlpi_hdr.h"

static void sigsegv_handler(int sig)
{
    int x;

    /* UNSAFE: printf, strsignal, fflush */
    std::printf("Caught sigal %d (%s)\n", sig, strsignal(sig));
    std::printf("Top of handler stack near %10p\n", &x);
    std::fflush(nullptr);
    _exit(EXIT_FAILURE);
}

static void overflow_stack(int call_num)
{
    char a[100000];

    std::printf("Call %4d - top of stack near %10p\n", call_num, &a[0]);
    overflow_stack(call_num + 1);
}

int main(int argc, char const *argv[])
{
    typedef struct sigaction sigaction_st;
    stack_t sigstack;
    sigaction_st sa;
    int j;
    std::printf("Top of standard stack is near %10p\n", &j);
    sigstack.ss_sp = std::malloc(SIGSTKSZ);
    if (sigstack.ss_sp == nullptr)
        err_exit("malloc");

    sigstack.ss_size = SIGSTKSZ;
    sigstack.ss_flags = 0;
    if (sigaltstack(&sigstack, nullptr))
        err_exit("sigaltstack");

    std::printf("Alternate stack is at %10p-%p\n", sigstack.ss_sp, static_cast<char *>(sbrk(0)) - 1);


    sa.sa_handler = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;
    if (sigaction(SIGSEGV, &sa, nullptr) == -1)
        err_exit("sigaction");

    overflow_stack(1);

    return 0;
}

