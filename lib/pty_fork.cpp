#include "pty_fork.h"
#include "pty_master_open.h"
#include "tlpi_hdr.h"
#include <fcntl.h>

#define MAX_SNAME 1000

pid_t pty_fork(int *master_fd, char *slave_name, size_t sn_len, const termios *slave_termios, const winsize *slave_winsize)
{
    char slname[MAX_SNAME];
    int mfd{ pty_master_open(slname, MAX_SNAME) };
    if (mfd == -1)
        return -1;

    if (slave_name != nullptr)
    {
        if (std::strlen(slname) < sn_len)
            std::strncpy(slave_name, slname, sn_len);
        else
        {
            close(mfd);
            errno = EOVERFLOW;
            return -1;
        }
    }

    pid_t child_pid{ fork() };

    if (child_pid == -1)
    {
        int saved_errno{ errno };
        close(mfd);
        errno = saved_errno;
        return -1;
    }

    if (child_pid != 0)
    {
        *master_fd = mfd;
        return child_pid; // parent return
    }

    if (setsid() == -1)
        err_exit("pty_fork:setsid");

    close(mfd);

    int slave_fd{ open(slname, O_RDWR) };
    if (slave_fd == -1)
        err_exit("pty_fork:open-slave");

#ifdef TIOCSCTTY
    if (ioctl(slave_fd, TIOCSCTTY, 0) == -1)
        err_exit("pty_fork:ioctl-TIOCSCTTY");
#endif

    if (slave_termios != nullptr)
    {
        if (tcsetattr(slave_fd, TCSANOW, slave_termios) == -1)
            err_exit("pty_fork:tcsetattr");
    }

    if (slave_winsize != nullptr)
    {
        if (ioctl(slave_fd, TIOCSWINSZ, slave_winsize) == -1)
            err_exit("pty_fork:ioctl-TIOCSWINSZ");
    }

    if (dup2(slave_fd, STDIN_FILENO) != STDIN_FILENO)
        err_exit("pty_fork:dup2-STDIN_FILENO");

    if (dup2(slave_fd, STDOUT_FILENO) != STDOUT_FILENO)
        err_exit("pty_fork:dup2-STDOUT_FILENO");

    if (dup2(slave_fd, STDERR_FILENO) != STDERR_FILENO)
        err_exit("pty_fork:dup2-STDERR_FILENO");

    if (slave_fd > STDERR_FILENO)
        close(slave_fd);

    return 0; // child return
}
