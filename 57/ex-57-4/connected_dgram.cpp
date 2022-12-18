#include "tlpi_hdr.h"
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>

#define ADD_TAIL(tail) "/tmp/ex-57-4-" tail

struct SockInfo
{
    int fd;
    sockaddr *addr;
    socklen_t len;
};

static int connect(SockInfo from, SockInfo to)
{
    return connect(from.fd, to.addr, to.len);
}

static SockInfo init_sock(const char *path)
{
    int fd{ socket(AF_UNIX, SOCK_DGRAM, 0) };
    if (fd == -1)
        err_exit("socket");

    sockaddr_un *addr{ new sockaddr_un{.sun_family{AF_UNIX}, .sun_path{""}} };
    std::strncpy(addr->sun_path, path, sizeof(sockaddr_un::sun_path) - 1);

    if (remove(path) == -1 && errno != ENOENT)
        err_exit("remove");

    if (bind(fd, reinterpret_cast<sockaddr *>(addr), sizeof(sockaddr_un)) == -1)
        err_exit("bind");

    return { fd, reinterpret_cast<sockaddr *>(addr), sizeof(sockaddr_un) };
}

SockInfo a, b, c;

void *sender_func(void *)
{
    constexpr size_t buf_size{ 10 };
    char buf[buf_size]{};
    if (sendto(c.fd, buf, buf_size, 0, a.addr, a.len) == -1)
        err_exit("sendto"); // Ubuntu 22.04 gives EPERM here

    return nullptr;
}

void *receiver_func(void *)
{
    constexpr size_t buf_size{ 10 };
    char buf[buf_size];
    if (recvfrom(a.fd, buf, buf_size, 0, nullptr, nullptr) == -1)
        err_exit("recvfrom");
    return nullptr;
}

int main(int argc, char *argv[])
{
    a = init_sock(ADD_TAIL("a")), b = init_sock(ADD_TAIL("b")), c = init_sock(ADD_TAIL("c"));
    if (argc == 1)
        connect(a, b);

    pthread_t sender_thread, receiver_thread;
    pthread_create(&sender_thread, nullptr, sender_func, nullptr);
    pthread_create(&receiver_thread, nullptr, receiver_func, nullptr);

    pthread_join(sender_thread, nullptr);
    pthread_join(receiver_thread, nullptr);

    remove(ADD_TAIL("a"));
    remove(ADD_TAIL("b"));
    remove(ADD_TAIL("c"));

    std::puts("Finished");

    return 0;
}
