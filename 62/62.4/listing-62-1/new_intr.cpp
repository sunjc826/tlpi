#include "tlpi_hdr.h"
#include <cctype>
#include <termios.h>

int main(int argc, char *argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0)
        usage_err("%s [intr-char]\n", argv[0]);

    int intr_char;
    if (argc == 1)
    {
        intr_char = fpathconf(STDIN_FILENO, _PC_VDISABLE);
        if (intr_char == -1)
            err_exit("Couldn't determine VDISABLE");
    }
    else if (isdigit(static_cast<unsigned char>(argv[1][0])))
        intr_char = strtoul(argv[1], nullptr, 0);
    else
        intr_char = argv[1][0];

    termios tp;
    if (tcgetattr(STDIN_FILENO, &tp) == -1)
        err_exit("tcgetattr");
    tp.c_cc[VINTR] = intr_char;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1)
        err_exit("tcsetattr");

    return 0;
}

