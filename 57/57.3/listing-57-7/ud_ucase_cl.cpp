#include "ud_case.h"
#include <climits>
constexpr const char *cl_sock_tmp{ "/tmp/ud_ucase_cl.%ld" };
constexpr size_t cl_sock_path_len{ sizeof(cl_sock_tmp) + 20 };
int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s msg...\n", argv[0]);

    char cl_sock_path[cl_sock_path_len];
    pid_t own_pid{ getpid() };
    if (own_pid == -1)
        err_exit("getpid");
    std::snprintf(cl_sock_path, cl_sock_path_len, cl_sock_tmp, static_cast<long>(own_pid));

    int client_fd{ socket(AF_UNIX, SOCK_DGRAM, 0) };
    if (client_fd == -1)
        err_exit("socket");

    sockaddr_un addr{ .sun_family{AF_UNIX}, .sun_path{""} };
    std::strncpy(addr.sun_path, cl_sock_path, sizeof(sockaddr_un::sun_path) - 1);

    if (bind(client_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(sockaddr_un)) == -1)
        err_exit("bind");

    sockaddr_un server_addr{ .sun_family{AF_UNIX}, .sun_path{""} };
    std::strncpy(server_addr.sun_path, sv_sock_path, sizeof(sockaddr_un::sun_path) - 1);
    socklen_t server_addr_size{ sizeof(server_addr) };
    char buf[buf_size];
    for (int i{ 1 }; i < argc; ++i)
    {
        ssize_t num_sent{ sendto(client_fd, argv[i], std::strlen(argv[i]), 0, reinterpret_cast<sockaddr *>(&server_addr), sizeof(sockaddr_un)) };
        if (num_sent != static_cast<ssize_t>(std::strlen(argv[i])))
            fatal("partial/failed sendto");

        ssize_t num_recved{ recvfrom(client_fd, buf, buf_size, 0, reinterpret_cast<sockaddr *>(&server_addr), &server_addr_size) };
        if (num_recved == -1)
            err_exit("recvfrom");

        std::printf("Response %d: %.*s\n", i, static_cast<int>(num_recved), buf);
    }

    if (close(client_fd) == -1)
        err_exit("close");

    remove(addr.sun_path);

    return 0;
}
