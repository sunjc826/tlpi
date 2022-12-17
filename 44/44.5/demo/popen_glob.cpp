#include "tlpi_hdr.h"
#include <climits>
constexpr size_t read_size{ PIPE_BUF };

int main(int argc, char *argv[])
{
    if (argc != 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s pathname\n", argv[0]);

    size_t len{ 2 + 1 + std::strlen(argv[1]) + 1 };
    char *cmd{ new char[len] };
    std::snprintf(cmd, len, "ls %s", argv[1]);

    FILE *output{ popen(cmd, "r") };
    if (output == nullptr)
        err_exit("popen");
    int fd{ fileno(output) };
    std::string str_output;
    char *read_buf{ new char[read_size + 1] };
    ssize_t num_read;
    while ((num_read = read(fd, read_buf, read_size)) > 0)
    {
        read_buf[num_read] = '\0';
        str_output += static_cast<const char *>(read_buf);
        // str_output = str_output + std::string{ read_buf, read_buf + num_read };
    }

    std::puts(str_output.c_str());
    delete[] read_buf;

    return 0;
}
