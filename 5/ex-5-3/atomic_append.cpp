// TODO: For some reason, even with O_APPEND, the output filesize is not correct
#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc < 3 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s filename num_bytes [x]", argv[0]);

    const char *filename{ argv[1] };
    size_t num_bytes{ static_cast<size_t>(get_long(argv[2], GN_ANY_BASE, argv[2])) };
    bool is_atomic_append{ argv[3] == nullptr };
    std::cout << "num_bytes " << num_bytes << std::boolalpha << ", is_atomic_append " << is_atomic_append << '\n';

    int fd{ open(filename, O_WRONLY | O_CREAT | (is_atomic_append ? O_APPEND : 0), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) };
    if (fd == -1)
        err_exit("open");
    char ch{ 'a' };
    for (size_t i{ 0 }; i < num_bytes; ++i)
    {
        if (!is_atomic_append)
            if (lseek(fd, 0, SEEK_END) == -1)
                err_exit("lseek");

        if (write(fd, &ch, 1) != 1)
            err_exit("write");
    }

    if (fsync(fd) == -1)
        err_exit("fsync");

    if (close(fd) == -1)
        err_exit("close");

    return 0;
}

