#define _DEFAULT_SOURCE
#include "tlpi_hdr.h"
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    hostent *h;
    char **pp;
    char str[INET6_ADDRSTRLEN];

    for (++argv; *argv != nullptr; ++argv)
    {
        h = gethostbyname(*argv);
        if (h == nullptr)
        {
            std::fprintf(stderr, "gethostbyname() failed for '%s': %s\n", *argv, hstrerror(h_errno));
            continue;
        }

        std::printf("Canonical name: %s\n", h->h_name);
        std::printf("\talias(es):   ");
        for (pp = h->h_aliases; *pp != nullptr; ++pp)
            std::printf(" %s", *pp);
        std::putchar('\n');
        std::printf("\taddress type: %s\n",
            (h->h_addrtype == AF_INET) ? "AF_INET" :
            (h->h_addrtype == AF_INET6) ? "AF_INET6" :
            "???");

        if (h->h_addrtype == AF_INET || h->h_addrtype == AF_INET6)
        {
            std::printf("\taddress(es): ");
            for (pp = h->h_addr_list; *pp != nullptr; ++pp)
                std::printf(" %s", inet_ntop(h->h_addrtype, *pp, str, INET6_ADDRSTRLEN));
            std::putchar('\n');
        }
    }

    return 0;
}

