#include "tty_functions.h"
#include "tlpi_hdr.h"

int tty_set_cbreak(int fd, termios *prev_termios)
{
    termios t;
    if (tcgetattr(fd, &t) == -1)
        return -1;
    if (prev_termios != nullptr)
        *prev_termios = t;

    t.c_lflag &= ~(ICANON | ECHO);
    t.c_lflag |= ISIG;
    t.c_iflag &= ~ICRNL;
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
        return -1;

    return 0;
}

int tty_set_raw(int fd, termios *prev_termios)
{
    termios t;
    if (tcgetattr(fd, &t) == -1)
        return -1;

    if (prev_termios != nullptr)
        *prev_termios = t;

    t.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
    t.c_iflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR | INPCK | ISTRIP | IXON | PARMRK);
    t.c_oflag &= ~OPOST;
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
        return -1;

    return 0;
}
