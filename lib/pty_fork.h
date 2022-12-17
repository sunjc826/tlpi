#ifndef PTY_FORK_H
#define PTY_FORK_H
#include <cstddef>
#include <sys/types.h>
#include <termios.h>
#include <sys/ioctl.h>
pid_t pty_fork(int *master_fd, char *slave_name, size_t sn_len,
    const termios *slave_termios, const winsize *slave_winsize);
#endif