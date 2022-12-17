#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "print_wait_status.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdio>

void print_wait_status(const char *msg, int status)
{
    if (msg != nullptr)
        std::printf("%s", msg);

    if (WIFEXITED(status))
        std::printf("child exited, status=%d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
    {
        std::printf("child killed by signal %d (%s)", WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP
        if (WCOREDUMP(status))
            std::printf(" (core dumped)");
        std::putchar('\n');
#endif
    }
    else if (WIFSTOPPED(status))
        std::printf("child stopped by signal %d (%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status)));
#ifdef WIFCONTINUED
    else if (WIFCONTINUED(status))
        ;
#endif
    else /* should not be reached */
        std::printf("what happened to this child? (status=%x)\n", static_cast<unsigned int>(status));
}
