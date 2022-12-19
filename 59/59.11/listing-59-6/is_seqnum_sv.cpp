#define _DEFAULT_SOURCE
#include "is_seqnum.h"
#include <netdb.h>

constexpr int backlog{ 50 };
constexpr int addr_str_len{ NI_MAXHOST + NI_MAXSERV + 10 };

int main(int argc, char *argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0)
        usage_err("%s [init-seq-num]\n", argv[0]);

    int seq_num{ (argc > 1) ? get_int(argv[1], 0, "init-seq-num") : 0 };
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        err_exit("signal");

    addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    addrinfo *result;
    if (getaddrinfo(nullptr, port_num, &hints, &result) != 0)
        err_exit("getaddrinfo");

    int optval{ 1 };
    int lfd;
    addrinfo *rp;
    for (rp = result; rp != nullptr; rp = rp->ai_next)
    {
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (lfd == -1)
            continue;
        if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
            err_exit("setsockopt");
        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(lfd);
    }

    if (rp == nullptr)
        fatal("Could not bind socket to any address");

    if (listen(lfd, backlog) == -1)
        err_exit("listen");

    freeaddrinfo(result);

    while (true)
    {
        sockaddr_storage claddr;
        socklen_t claddr_size{ sizeof(sockaddr_storage) };
        int cfd{ accept(lfd, reinterpret_cast<sockaddr *>(&claddr), &claddr_size) };
        if (cfd == -1)
        {
            err_msg("accept");
            continue;
        }

        char nameinfo_host[NI_MAXHOST];
        char nameinfo_serv[NI_MAXSERV];
        char addr_str[addr_str_len];
        if (getnameinfo(reinterpret_cast<sockaddr *>(&claddr), claddr_size, nameinfo_host, NI_MAXHOST, nameinfo_serv, NI_MAXSERV, 0) == 0)
            std::snprintf(addr_str, addr_str_len, "(%s, %s)", nameinfo_host, nameinfo_serv);
        else
            std::snprintf(addr_str, addr_str_len, "(?UNKNOWN?)");
        std::printf("Connection from %s\n", addr_str);

        char req_len_str[int_len];
        char seq_num_str[int_len];
        if (read_line(cfd, req_len_str, int_len) <= 0)
        {
            close(cfd);
            continue;
        }

        int req_len{ atoi(req_len_str) };
        if (req_len <= 0)
        {
            std::printf("Misbehaving client: %d\n", req_len);
            close(cfd);
            continue;
        }

        std::snprintf(seq_num_str, int_len, "%d\n", seq_num);
        if (write(cfd, &seq_num_str, std::strlen(seq_num_str)) != static_cast<ssize_t>(std::strlen(seq_num_str)))
            std::fprintf(stderr, "Error on write");

        seq_num += req_len;
        if (close(cfd) == -1)
            err_msg("close");
    }

    return 0;
}

