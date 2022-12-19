#include "i6d_ucase.h"

int main(int argc, char *argv[])
{
    int sfd{ socket(AF_INET6, SOCK_DGRAM, 0) };
    if (sfd == -1)
        err_exit("socket");

    sockaddr_in6 svaddr;
    std::memset(&svaddr, 0, sizeof(sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_addr = in6addr_any;
    svaddr.sin6_port = htons(port_num);

    if (bind(sfd, reinterpret_cast<sockaddr *>(&svaddr), sizeof(sockaddr_in6)) == -1)
        err_exit("bind");

    char buf[buf_size];
    char claddr_str[INET6_ADDRSTRLEN];
    while (true)
    {
        sockaddr_in6 claddr;
        socklen_t len{ sizeof(sockaddr_in6) };
        ssize_t num_bytes{ recvfrom(sfd, buf, buf_size, 0, reinterpret_cast<sockaddr *>(&claddr), &len) };
        if (num_bytes == -1)
            err_exit("recvfrom");

        if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddr_str, INET6_ADDRSTRLEN) == nullptr)
            std::puts("Couldn't convert client address to string");
        else
            std::printf("Server received %ld bytes from (%s, %u)\n", static_cast<long>(num_bytes), claddr_str, ntohs(claddr.sin6_port));

        for (int i{}; i < num_bytes; ++i)
            buf[i] = toupper(buf[i]);

        if (sendto(sfd, buf, num_bytes, 0, reinterpret_cast<sockaddr *>(&claddr), len) != num_bytes)
            fatal("sendto");
    }

    return 0;
}

