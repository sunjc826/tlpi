#include "i6d_ucase.h"

int main(int argc, char *argv[])
{

    if (argc < 3 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s host-address msg...\n", argv[0]);

    int sfd{ socket(AF_INET6, SOCK_DGRAM, 0) };
    if (sfd == -1)
        err_exit("socket");

    sockaddr_in6 svaddr;
    memset(&svaddr, 0, sizeof(sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_port = htons(port_num);
    if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0)
        fatal("inet_pton failed for address '%s'", argv[1]);

    char resp[buf_size];
    for (int i{ 2 }; i < argc; ++i)
    {
        size_t msg_len{ std::strlen(argv[i]) };
        if (sendto(sfd, argv[i], msg_len, 0, reinterpret_cast<sockaddr *>(&svaddr), sizeof(sockaddr_in6)) != static_cast<ssize_t>(msg_len))
            fatal("sendto");

        ssize_t num_bytes{ recvfrom(sfd, resp, buf_size, 0, nullptr, nullptr) };
        if (num_bytes == -1)
            err_exit("recvfrom");

        std::printf("Response %d: %.*s\n", i - 1, static_cast<int>(num_bytes), resp);
    }

    return 0;
}

