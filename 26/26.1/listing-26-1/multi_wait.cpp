#include "tlpi_hdr.h"
#include "curr_time.h"
#include <ctime>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s sleep-time...\n", argv[0]);

    setbuf(stdout, nullptr);
    for (int i{ 1 }; i < argc; ++i)
    {
        switch (fork())
        {
        case -1:
            err_exit("fork");
            break;
        case 0:
            std::printf("[%s] child %d started with PID %ld, sleeping %s seconds\n",
                curr_time("%T"), i, static_cast<long>(getpid()), argv[i]);
            sleep(get_int(argv[i], GN_NONNEG, "sleep-time"));
            _exit(EXIT_SUCCESS);
            break;
        default:
            break;
        }
    }

    int num_dead{ 0 };
    while (true)
    {
        int child_pid{ wait(nullptr) };
        if (child_pid == -1)
        {
            if (errno == ECHILD)
            {
                std::printf("No more children - bye!\n");
                std::exit(EXIT_SUCCESS);
            }
            else
                err_exit("wait");
        }

        ++num_dead;
        std::printf("[%s] wait() returned child PID %ld (num_dead=%d)\n",
            curr_time("%T"), static_cast<long>(child_pid), num_dead);
    }


    return 0;
}

