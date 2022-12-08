#include "tlpi_hdr.h"
#include <fcntl.h>

constexpr std::size_t cmd_size{ 200u };
constexpr std::size_t buf_size{ 1024u };

int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s temp-file [num-1kB-blocks] \n", argv[0]);

    int num_blocks{ (argc > 2) ? get_int(argv[2], GN_GT_0, "num-1kB-blocks") : 10000 };

    int fd{ open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR) };
    if (fd == -1)
        err_exit("open");

    if (unlink(argv[1]) == -1)
        err_exit("unlink");
    char buf[buf_size];
    for (int i{ 0 }; i < num_blocks; i++)
        if (write(fd, buf, buf_size) != buf_size)
            fatal("partial/failed write");

    char shell_cmd[cmd_size];
    std::snprintf(shell_cmd, cmd_size, "df -k $(dirname %s)", argv[1]);
    std::system(shell_cmd);

    if (close(fd) == -1)
        err_exit("close");
    std::printf("********* Closed file descriptor\n");

    std::system(shell_cmd);

    return 0;
}

