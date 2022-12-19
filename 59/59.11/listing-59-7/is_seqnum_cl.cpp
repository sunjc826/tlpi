#include "is_seqnum.h"
#include <netdb.h>

int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s server-host [sequence-len]\n", argv[0]);

    addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    addrinfo *result;
    if (getaddrinfo(argv[1], port_num, &hints, &result) != 0)
        err_exit("getaddrinfo");

    int cfd;
    addrinfo *rp;
    for (rp = result; rp != nullptr; rp = rp->ai_next)
    {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cfd == -1)
            continue;

        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;

        close(cfd);
    }

    if (rp == nullptr)
        fatal("Could not connect socket to any address");

    freeaddrinfo(result);

    const char *req_len_str{ (argc > 2) ? argv[2] : "1" };
    if (write(cfd, req_len_str, std::strlen(req_len_str)) != static_cast<ssize_t>(std::strlen(req_len_str)))
        fatal("Partial/failed write (req_len_str)");
    if (write(cfd, "\n", 1) != 1)
        fatal("Partial/failed write (newline)");

    char seq_num_str[int_len];
    ssize_t num_read{ read_line(cfd, seq_num_str, int_len) };
    if (num_read == -1)
        err_exit("read_line");
    if (num_read == 0)
        fatal("Unexpected EOF from server");

    std::printf("Sequence number: %s", seq_num_str);

    return 0;
}
