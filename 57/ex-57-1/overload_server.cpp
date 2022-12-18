#include "tlpi_hdr.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>

constexpr const char *server_sock_path{ "/tmp/ex-57-1_sv" };
constexpr const char *client_sock_path{ "/tmp/ex-57-1_cl" };

static void handler(int)
{
}

static int init_sock(const char *path)
{
    int fd{ socket(AF_UNIX, SOCK_DGRAM, 0) };
    if (fd == -1)
        err_exit("socket");

    sockaddr_un addr{ .sun_family{AF_UNIX}, .sun_path{""} };
    std::strncpy(addr.sun_path, path, sizeof(sockaddr_un::sun_path) - 1);

    if (remove(path) == -1 && errno != ENOENT)
        err_exit("remove");

    if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(sockaddr_un)) == -1)
        err_exit("bind");

    return fd;
}


void server_func(void *args)
{
    int server_fd{ init_sock(server_sock_path) };
    size_t buf_size{ 4096 };
    int sleep_time{ reinterpret_cast<size_t>(args) };
    char buf[buf_size];
    sockaddr_un client_addr;
    sockaddr *client_addrp{ reinterpret_cast<sockaddr *>(&client_addr) };
    socklen_t client_addr_size{ sizeof(client_addr) };
    while (true)
    {
        ssize_t num_recved;
        if ((num_recved = recvfrom(server_fd, buf, buf_size, 0, client_addrp, &client_addr_size)) == -1)
            err_exit("recvfrom");
        std::printf("Server: Received %ld bytes\n", static_cast<long>(num_recved));
        sleep(sleep_time);
    }

}

void client_func(void *args)
{
    int client_fd{ init_sock(client_sock_path) };
    size_t buf_size{ 4096 };
    int sleep_time{ reinterpret_cast<size_t>(args) };
    char buf[buf_size];
    sockaddr_un server_addr{ .sun_family{AF_UNIX}, .sun_path{""} };
    sockaddr *server_addrp{ reinterpret_cast<sockaddr *>(&server_addr) };
    std::strncpy(server_addr.sun_path, server_sock_path, sizeof(sockaddr_un::sun_path) - 1);
    while (true)
    {
        ssize_t num_sent;
        if ((num_sent = sendto(client_fd, buf, buf_size, 0, server_addrp, sizeof(server_addr))) == -1)
            err_exit("sendto");
        std::printf("Client: Sent %ld bytes\n", static_cast<long>(num_sent));
        sleep(sleep_time);
    }

}


int main(int argc, char const *argv[])
{
    if (argc < 3 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s <server_sleep_time> <client_sleep_time>\n", argv[0]);
    int server_sleep_time{ get_int(argv[1], GN_GT_0, "server_sleep_time") };
    int client_sleep_time{ get_int(argv[2], GN_GT_0, "client_sleep_time") };
    pid_t server_pid, client_pid;
    switch (server_pid = fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        server_func(reinterpret_cast<void *>(server_sleep_time));
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }

    switch (client_pid = fork())
    {
    case -1:
        err_exit("fork");
        break;
    case 0:
        client_func(reinterpret_cast<void *>(client_sleep_time));
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }

    sigaction_st sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, nullptr) == -1)
        err_exit("sigaction");

    pause();

    kill(server_pid, SIGINT);
    kill(client_pid, SIGINT);
    if (wait(nullptr) == -1)
        err_exit("wait");
    if (wait(nullptr) == -1)
        err_exit("wait");

    remove(server_sock_path);
    remove(client_sock_path);

    return 0;
}
