#include "us_xfr.h"

constexpr int backlog{ 5 };

int main(int argc, char *argv[])
{
    int sfd{ socket(AF_UNIX, SOCK_STREAM, 0) };
    if (sfd == -1)
        err_exit("socket");

    if (remove(sv_sock_path) == -1 && errno != ENOENT)
        err_exit("remove-%s", sv_sock_path);

    sockaddr_un addr;
    std::memset(&addr, 0, sizeof(sockaddr_un));
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, sv_sock_path, sizeof(sockaddr_un::sun_path) - 1);

    if (bind(sfd, reinterpret_cast<sockaddr *>(&addr), sizeof(sockaddr_un)) == -1)
        err_exit("bind");

    if (listen(sfd, backlog) == -1)
        err_exit("listen");

    while (true)
    {
        // connection fd
        int cfd{ accept(sfd, nullptr, nullptr) };
        if (cfd == -1)
            err_exit("accept");

        int num_read;
        char buf[buf_size];
        while ((num_read = read(cfd, buf, buf_size)) > 0)
        {
            if (write(STDOUT_FILENO, buf, num_read) != num_read)
                fatal("partial/failed write");
        }

        if (num_read == -1)
            err_exit("read");

        if (close(cfd) == -1)
            err_exit("close");
    }

    return 0;
}

