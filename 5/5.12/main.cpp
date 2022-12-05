#include "tlpi_hdr.h"

void create_temp_file()
{
    char template_str[] = "/tmp/somestringXXXXXX";
    int fd{ mkstemp(template_str) };
    if (fd == -1)
        err_exit("mkstemp");

    std::printf("Generated filename was: %s\n", template_str);
    unlink(template_str);

    /* Do file I/O on fd */

    if (close(fd) == -1)
        err_exit("close");
}
