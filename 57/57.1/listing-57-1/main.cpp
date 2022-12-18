#include "tlpi_hdr.h"
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[])
{
    const char *sockname{ "/tmp/mysock" };
    int sfd{ socket(AF_UNIX, SOCK_STREAM, 0) };
    if (sfd == -1)
        err_exit("socket");

    sockaddr_un addr;
    std::memset(&addr, 0, sizeof(sockaddr_un));
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, sockname, sizeof(addr.sun_path) - 1);

    if (bind(sfd, reinterpret_cast<sockaddr *>(&addr), sizeof(sockaddr_un) == -1))
        err_exit("bind");

    return 0;
}
