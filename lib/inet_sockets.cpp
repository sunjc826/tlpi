#include "inet_sockets.h"
#include "tlpi_hdr.h"
#include <netinet/in.h>
#include <arpa/inet.h>

int inet_connect(const char *host, const char *service, int type)
{
    addrinfo hint{
        .ai_family{AF_UNSPEC},
        .ai_socktype{type}
    };
    addrinfo *result;
    if (getaddrinfo(host, service, &hint, &result) != 0)
    {
        errno = ENOSYS;
        return -1;
    }
    addrinfo *infop;
    int client_fd;
    for (infop = result; infop != nullptr; infop = infop->ai_next)
    {
        client_fd = socket(infop->ai_family, infop->ai_socktype, infop->ai_protocol);
        if (client_fd == -1)
            continue;

        if (connect(client_fd, infop->ai_addr, infop->ai_addrlen) == 0)
            break;

        close(client_fd);
    }

    freeaddrinfo(result);

    if (infop == nullptr)
        return -1;

    return client_fd;
}

static int inet_passive_socket(const char *service, int type, socklen_t *addr_len, bool do_listen, int backlog)
{
    addrinfo hint{
        .ai_flags{AI_PASSIVE},
        .ai_family{AF_UNSPEC},
        .ai_socktype{SOCK_STREAM},
    };

    addrinfo *result;
    if (getaddrinfo(nullptr, service, &hint, &result) != 0)
        return -1;

    addrinfo *infop;
    int server_fd;
    int optval{ 1 };
    for (infop = result; infop != nullptr; infop = infop->ai_next)
    {
        server_fd = socket(infop->ai_family, infop->ai_socktype, infop->ai_protocol);
        if (server_fd == -1)
            continue;

        if (do_listen)
        {
            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
            {
                close(server_fd);
                freeaddrinfo(result);
                return -1;
            }
        }

        if (bind(server_fd, infop->ai_addr, infop->ai_addrlen) == 0)
            break;

        close(server_fd);
    }

    if (infop != nullptr)
    {
        if (do_listen)
        {
            if (listen(server_fd, backlog) == -1)
            {
                close(server_fd);
                freeaddrinfo(result);
                return -1;
            }
        }

        if (addr_len != nullptr)
            *addr_len = infop->ai_addrlen;
    }




    freeaddrinfo(result);
    if (infop == nullptr)
        return -1;



    return server_fd;
}

int inet_listen(const char *service, int backlog, socklen_t *addr_len)
{
    return inet_passive_socket(service, SOCK_STREAM, addr_len, true, backlog);
}

int inet_bind(const char *service, int type, socklen_t *addr_len)
{
    return inet_passive_socket(service, type, addr_len, false, 0);
}

char *inet_address_str(const sockaddr *addr, socklen_t addr_len, char *addr_str, int addr_str_len)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    if (getnameinfo(addr, addr_len, host, NI_MAXHOST, serv, NI_MAXSERV, 0) == 0)
        std::snprintf(addr_str, addr_str_len, "(%s, %s)", host, serv);
    else
        std::snprintf(addr_str, addr_str_len, "(?UNKNOWN?)");

    addr_str[addr_str_len - 1] = '\0';

    return addr_str;
}
