// read from standard input
// write to standard output
// and write to file
#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s {-a} <filename>", argv[0]);

    bool is_append{};
    int opt;
    /*
    By default, getopt() permutes the contents of argv as it scans, so that eventually all the nonoptions are at the end.

    Apparently, this is glibc specific behavior. POSIX will stop scanning the options as soon as the first non-option argument is met.
    */
    while ((opt = getopt(argc, argv, "a")) != -1)
    {
        if (opt == static_cast<int>('a'))
            is_append = true;
    }

    constexpr size_t buf_size{ 5000 };
    char buf[buf_size];

    // read from stdin
    ssize_t num_read{ read(STDIN_FILENO, buf, buf_size) };
    if (num_read == -1)
        err_exit("read");

    // write to stdout
    ssize_t num_written;
    num_written = write(STDOUT_FILENO, buf, static_cast<size_t>(num_read));
    if (num_written != num_read)
        err_exit("Did not write everything");

    // open file and write to it
    const char *filename{ argv[optind] };
    int fd{ open(filename, O_WRONLY | O_CREAT | (is_append ? O_APPEND : O_TRUNC), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) };
    if (fd == -1)
        err_exit("open");
    num_written = write(fd, buf, static_cast<size_t>(num_read));
    if (num_written != num_read)
        err_exit("Did not write everything");

    return 0;
}

