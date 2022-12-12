#include "tlpi_hdr.h"
#include <sys/wait.h>
int func();

// Calling a function without changing the process's memory footprint.
int main(int argc, char const *argv[])
{
    pid_t child_pid;
    int status;

    child_pid = fork();
    if (child_pid == -1)
        err_exit("fork");

    if (child_pid == 0)
        exit(func());

    if (wait(&status) == -1)
        err_exit("wait");

    return 0;
}

