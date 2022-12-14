#include "tlpi_hdr.h"
#include "print_wait_status.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0)
        usage_err("%s [exit-status]\n", argv[0]);

    switch (fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        std::printf("Child started with PID=%ld\n",
            static_cast<long>(getpid()));
        if (argc > 1)
            std::exit(get_int(argv[1], 0, "exit-status"));
        else
        {
            while (true)
                pause();
        }
        std::exit(EXIT_FAILURE);
        break;
    default:
        while (true)
        {
            int status;
            pid_t child_pid{ waitpid(-1, &status, WUNTRACED | WCONTINUED) };
            if (child_pid == -1)
                err_exit("waitpid");
            std::printf("waitpid() returned: PID=%ld; status=0x%04x (%d,%d)",
                static_cast<long>(child_pid), static_cast<unsigned int>(status), status >> 8, status & 0xff);
            print_wait_status(nullptr, status);

            if (WIFEXITED(status) || WIFSIGNALED(status))
                std::exit(EXIT_SUCCESS);
        }


        break;
    }
    return 0;
}


