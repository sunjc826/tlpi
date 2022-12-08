#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <climits>

constexpr std::size_t buf_size{ PATH_MAX };
int main(int argc, char const *argv[])
{
    if (argc != 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s pathname\n", argv[0]);

    typedef struct stat stat_st;
    stat_st statbuf;
    if (lstat(argv[1], &statbuf) == -1)
        err_exit("lstat");

    if (!S_ISLNK(statbuf.st_mode))
        fatal("%s is not a symbolic link", argv[1]);

    char buf[buf_size];
    ssize_t num_bytes{ readlink(argv[1], buf, buf_size - 1) };
    if (num_bytes == -1)
        err_exit("readlink");
    buf[num_bytes] = '\0';
    std::printf("readlink: %s --> %s\n", argv[1], buf);

    if (realpath(argv[1], buf) == nullptr)
        err_exit("realpath");
    std::printf("realpath: %s --> %s\n", argv[1], buf);

    return 0;
}
