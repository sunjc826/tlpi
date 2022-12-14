#include "tlpi_hdr.h"
#include <signal.h>
#include <libgen.h>

constexpr int cmd_size{ 200 };

int main(int argc, char *argv[])
{
    char cmd[cmd_size];
    pid_t child_pid;

    std::setbuf(stdout, nullptr);

    std::printf("Parent PID=%ld\n", static_cast<long>(getpid()));

    switch (child_pid = fork())
    {
    case -1:
        err_exit("fork");
        break;

    case 0:
        std::printf("Child (PID=%ld) exiting\n", static_cast<long>(getpid()));
        _exit(EXIT_SUCCESS);
        break;

    default:
        sleep(3);
        std::snprintf(cmd, cmd_size, "ps | grep %s", basename(argv[0]));
        cmd[cmd_size - 1] = '\0';
        std::system(cmd);

        if (kill(child_pid, SIGKILL) == -1)
            err_exit("kill");

        std::printf("After sending SIGKILL to zombie (PID=%ld):\n",
            static_cast<long>(child_pid));
        std::system(cmd);
        break;
    }

    return 0;
}

