#ifndef TTY_FUNCTIONS_H
#define TTY_FUNCTIONS_H
#include <termios.h>

int tty_set_cbreak(int fd, termios *prev_termios);
int tty_set_raw(int fd, termios *prev_termios);
#endif