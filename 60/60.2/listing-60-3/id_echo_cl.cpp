#include "id_echo.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s: host msg...\n", argv[0]);

    int server_fd{ inet_connect(argv[1], service, SOCK_DGRAM) };
    if (server_fd == -1)
        fatal("Could not connect to server socket");

    char buf[buf_size];
    for (int i{ 2 }; i < argc; ++i)
    {
        size_t len{ std::strlen(argv[i]) };
        if (write(server_fd, argv[i], len) != static_cast<ssize_t>(len))
            fatal("partial/failed write");

        ssize_t num_read{ read(server_fd, buf, buf_size) };
        if (num_read == -1)
            err_exit("read");

        std::printf("[%ld bytes] %.*s\n", static_cast<long>(num_read), static_cast<int>(num_read), buf);
    }

    return 0;
}
