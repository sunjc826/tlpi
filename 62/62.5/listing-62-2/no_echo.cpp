#include "tlpi_hdr.h"
#include <termios.h>

constexpr int buf_size{ 100 };

int main(int argc, char *argv[])
{
    termios term_attr, save_attr;
    if (tcgetattr(STDIN_FILENO, &term_attr) == -1)
        err_exit("tcgetattr");
    save_attr = term_attr;
    term_attr.c_lflag &= ~ECHO;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_attr) == -1)
        err_exit("tcsetattr");

    std::printf("Enter text: ");
    std::fflush(stdout);
    char buf[buf_size];
    if (std::fgets(buf, buf_size, stdin) == nullptr)
        std::puts("Got end-of-file/error on fgets()");
    else
        std::printf("\nRead: %s", buf);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &save_attr) == -1)
        err_exit("tcsetattr");

    return 0;
}

