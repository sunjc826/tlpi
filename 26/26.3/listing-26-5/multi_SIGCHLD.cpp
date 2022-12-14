#include "tlpi_hdr.h"
#include <sys/wait.h>
#include "print_wait_status.h"
#include "curr_time.h"

static volatile int num_live_children{};

static void sigchld_handler(int sig)
{
    /* UNSAFE:
    printf, print_wait_status, curr_time
    */

    int saved_errno{ errno };

    std::printf("%s handler: Caught SIGCHLD\n", curr_time("%T"));

    pid_t child_pid;
    int status;
    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        std::printf("%s handler: Reaped child %ld - ", curr_time("%T"), static_cast<long>(child_pid));
        print_wait_status(nullptr, status);
        --num_live_children;
    }

    if (child_pid == -1 && errno != ECHILD)
        err_msg("waitpid");

    sleep(5);
    std::printf("%s handler: returning\n", curr_time("%T"));

    errno = saved_errno;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s child-sleep-time...\n", argv[0]);

    std::setbuf(stdout, nullptr);

    int sig_cnt{};
    num_live_children = argc - 1;

    sigaction_st sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchld_handler;
    if (sigaction(SIGCHLD, &sa, nullptr) == -1)
        err_exit("sigaction");

    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &block_mask, nullptr) == -1)
        err_exit("sigprocmask");

    for (int i{ 1 }; i < argc; ++i)
    {
        switch (fork())
        {
        case -1:
            err_exit("fork");
            break;
        case 0:
            sleep(static_cast<unsigned>(get_int(argv[i], GN_NONNEG, "child-sleep-time")));
            std::printf("%s Child %d (PID=%ld) exiting\n", curr_time("%T"), i, static_cast<long>(getpid()));
            _exit(EXIT_SUCCESS);
            break;
        default:
            break;
        }
    }

    sigset_t empty_set;
    sigemptyset(&empty_set);
    while (num_live_children > 0)
    {
        if (sigsuspend(&empty_set) == -1 && errno != EINTR)
            err_exit("sigsuspend");
        ++sig_cnt;
    }

    std::printf("%s All %d children have terminated; SIGCHLD was caught "
        "%d times\n", curr_time("%T"), argc - 1, sig_cnt);

    return 0;
}

