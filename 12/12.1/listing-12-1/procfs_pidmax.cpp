#include "tlpi_hdr.h"
#include <fcntl.h>

constexpr int max_line{ 100 };

int main(int argc, char *argv[])
{
    int fd{ open("/proc/sys/kernel/pid_max", (argc > 1) ? O_RDWR : O_RDONLY) };
    if (fd == -1)
        err_exit("open");

    char *line[max_line];
    int num_read{ static_cast<int>(read(fd, line, max_line)) };
    if (num_read == -1)
        err_exit("read");

    if (argc > 1)
        std::printf("Old value: ");
    std::printf("%.*s", num_read, line);

    if (argc > 1)
    {
        if (write(fd, argv[1], strlen(argv[1])) != strlen(argv[1]))
            fatal("write() failed");

        system(
            "echo /proc/sys/kernel/pid_max now contains "
            "$(cat /proc/sys/kernel/pid_max)"
        );
    }

    return 0;
}
