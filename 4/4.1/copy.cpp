#include "tlpi_hdr.h"
#include "error_functions.h"
#include <sys/stat.h>
#include <fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char const *argv[])
{
    constexpr int buf_size{ BUF_SIZE };

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usage_err("%s old-file new-file\n", argv[0]);

    int input_fd{ open(argv[1], O_RDONLY) };
    if (input_fd == -1)
        err_exit("opening file %s", argv[1]);

    int output_fd{ open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) };
    if (output_fd == -1)
        err_exit("opening file %s", argv[2]);

    char buf[buf_size];
    int num_read;
    while ((num_read = read(input_fd, buf, buf_size)) > 0)
        if (write(output_fd, buf, static_cast<size_t>(num_read)) != num_read)
            fatal("couldn't write whole buffer");

    if (num_read == -1)
        err_exit("read");

    if (close(input_fd) == -1)
        err_exit("close input");

    if (close(output_fd) == -1)
        err_exit("close output");

    return 0;
}


