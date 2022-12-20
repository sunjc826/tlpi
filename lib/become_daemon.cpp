#include "become_daemon.h"
#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int become_daemon(int flags)
{
    switch (fork())
    {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }
    // Note that even though this process is in an orphaned process group
    // there is no SIGHUP sent by the kernel if all members of this orphaned process group are not stopped

    if (setsid() == -1)
        return -1;
    // I am now a standalone process in a new session
    switch (fork())
    {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    // I am now a standalone process in a session, and I am not the session leader.
    // As a result, I will not be able to acquire a controlling terminal when opening a terminal,
    // even if O_NOCTTY is not set


    if (!(flags & BD_NO_UMASK0))
        umask(0);

    if (!(flags & BD_NO_CHDIR))
        chdir("/");

    if (!(flags & BD_NO_CLOSE_FILES))
    {
        int maxfd{ sysconf(_SC_OPEN_MAX) };
        if (maxfd == -1) // indeterminate limit
            maxfd = BD_MAX_CLOSE;

        for (int fd{ 0 }; fd < maxfd; fd++)
            close(fd);
    }

    if (!(flags & BD_NO_REOPEN_STD_FDS))
    {
        close(STDIN_FILENO);

        int dev_null_fd{ open("/dev/null", O_RDWR) };
        if (dev_null_fd != STDIN_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    return 0;
}