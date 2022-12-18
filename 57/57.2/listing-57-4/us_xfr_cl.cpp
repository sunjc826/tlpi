#include "us_xfr.h"

int main(int argc, char *argv[])
{
    int sfd{ socket(AF_UNIX, SOCK_STREAM, 0) };
    if (sfd == -1)
        err_exit("socket");

    sockaddr_un addr{ .sun_family{AF_UNIX},.sun_path{""} };
    std::strncpy(addr.sun_path, sv_sock_path, sizeof(sockaddr_un::sun_path) - 1);

    if (connect(sfd, reinterpret_cast<sockaddr *>(&addr), sizeof(sockaddr_un)) == -1)
        err_exit("connect");

    int num_read;
    char buf[buf_size];
    while ((num_read = read(STDIN_FILENO, buf, buf_size)) > 0)
    {
        if (write(sfd, buf, num_read) != num_read)
            fatal("partial/failed write");
    }

    if (num_read == -1)
        err_exit("read");

    if (close(sfd) == -1)
        err_exit("close");

    return 0;
}
