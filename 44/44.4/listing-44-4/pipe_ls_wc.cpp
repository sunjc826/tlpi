#include "tlpi_hdr.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        err_exit("pipe");

    // ls
    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        if (close(pipefd[0]) == -1)
            err_exit_("close 1");

        if (pipefd[1] != STDOUT_FILENO)
        {
            if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                err_exit_("dup2");
            if (close(pipefd[1]) == -1)
                err_exit_("close 2");
        }

        execlp("ls", "ls", nullptr);
        err_exit_("execlp ls");
        break;
    default:
        break;
    }

    // wc
    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        if (close(pipefd[1]) == -1)
            err_exit_("close 3");

        if (pipefd[0] != STDIN_FILENO)
        {
            if (dup2(pipefd[0], STDIN_FILENO) == -1)
                err_exit_("dup2");
            if (close(pipefd[0]) == -1)
                err_exit_("close 4");
        }
        execlp("wc", "wc", nullptr);
        err_exit_("execlp wc");
        break;
    default:
        break;
    }

    if (close(pipefd[0]) == -1)
        err_exit("close 5");
    if (close(pipefd[1]) == -1)
        err_exit("close 6");

    if (wait(nullptr) == -1)
        err_exit("wait 1");
    if (wait(nullptr) == -1)
        err_exit("wait 2");
    return 0;
}

