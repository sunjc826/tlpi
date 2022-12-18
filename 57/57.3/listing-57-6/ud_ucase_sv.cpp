#include "ud_case.h"

int main(int argc, char *argv[])
{
    int server_fd{ socket(AF_UNIX, SOCK_DGRAM, 0) };
    if (server_fd == -1)
        err_exit("socket");

    if (remove(sv_sock_path) == -1 && errno != ENOENT)
        err_exit("remove-%s", sv_sock_path);

    sockaddr_un addr{ .sun_family{AF_UNIX}, .sun_path{""} };
    std::strncpy(addr.sun_path, sv_sock_path, sizeof(sockaddr_un::sun_path) - 1);
    if (bind(server_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(sockaddr_un)) == -1)
        err_exit("bind");

    char buf[buf_size];
    sockaddr_un client_addr;
    socklen_t addr_buf_size{ sizeof(sockaddr_un) };
    while (true)
    {
        ssize_t num_recved{ recvfrom(server_fd, buf, buf_size, 0, reinterpret_cast<sockaddr *>(&client_addr), &addr_buf_size) };
        if (num_recved == -1)
            err_exit("recvfrom");

        std::printf("Server received %ld bytes from %s\n", static_cast<long>(num_recved), client_addr.sun_path);

        for (int i{}; i < num_recved; ++i)
            buf[i] = toupper(buf[i]);

        ssize_t num_sent{ sendto(server_fd, buf, num_recved, 0, reinterpret_cast<sockaddr *>(&client_addr), addr_buf_size) };
        if (num_sent != num_recved)
            fatal("partial/failed sendto");
    }


    return 0;
}

