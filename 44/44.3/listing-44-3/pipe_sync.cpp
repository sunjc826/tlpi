#include "tlpi_hdr.h"
#include "curr_time.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s sleep-time...\n", argv[0]);

    std::setbuf(stdout, nullptr);

    std::printf("%s Parent started\n", curr_time("%T"));

    int pfd[2];
    if (pipe(pfd) == -1)
        err_exit("pipe");

    for (int i{ 1 }; i < argc; ++i)
    {
        switch (fork())
        {
        case -1:
            err_exit("fork %d", i);
            break;
        case 0:
            if (close(pfd[0]) == -1)
                err_exit_("close");

            sleep(get_int(argv[i], GN_NONNEG, "sleep-time"));

            std::printf("%s  Child %d (PID=%ld) closing pipe\n", curr_time("%T"), i, static_cast<long>(getpid()));

            if (close(pfd[1]) == -1)
                err_exit("close");

            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }

    if (close(pfd[1]) == -1)
        err_exit("close");

    int dummy;
    if (read(pfd[0], &dummy, 1) != 0)
        fatal("parent didn't get EOF");

    std::printf("%s  Parent ready to go\n", curr_time("%T"));
    return 0;
}

