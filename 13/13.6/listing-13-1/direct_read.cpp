#define _GNU_SOURCE
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <malloc.h>

int main(int argc, char const *argv[])
{
    if (argc < 3 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s file length [offset [alignment]]\n", argv[0]);

    long length{ get_long(argv[2], GN_ANY_BASE, "length") };
    long offset{ (argc > 3) ? get_long(argv[3], GN_ANY_BASE, "offset") : 0 };
    long alignment{ (argc > 4) ? get_long(argv[4], GN_ANY_BASE, "alignment") : 4096 };

    int fd{ open(argv[1], O_RDONLY | O_DIRECT) };
    if (fd == -1)
        err_exit("open");

    char *buf{ static_cast<char *>(memalign(alignment * 2, length + alignment)) + alignment };
    if (buf == nullptr)
        err_exit("memalign");

    if (lseek(fd, offset, SEEK_SET) == -1)
        err_exit("lseek");

    ssize_t num_read{ read(fd, buf, length) };
    if (num_read == -1)
        err_exit("read");

    std::printf("Read %ld bytes\n", static_cast<long>(num_read));

    std::free(buf - alignment);

    return 0;
}

