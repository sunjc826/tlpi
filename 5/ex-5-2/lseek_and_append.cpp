#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    if (argc != 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s filename", argv[0]);

    int fd{ open(argv[1], O_WRONLY | O_APPEND) };
    if (fd == -1)
        err_exit("open");

    if (lseek(fd, 0, SEEK_SET) == -1)
        err_exit("lseek");
    const char *str_to_append{ "String to append" };
    if (write(fd, str_to_append, std::strlen(str_to_append)) == -1)
        err_exit("write");

    return 0;
}
