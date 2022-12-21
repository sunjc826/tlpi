#include "id_echo.h"
#include "become_daemon.h"
#include <syslog.h>

int main(int argc, char *argv[])
{
    if (become_daemon(0) == -1)
        err_exit("become_daemon");

    socklen_t addr_len;
    // This application needs to be run using sudo as we are binding to a reserved port
    int server_fd{ inet_bind(service, SOCK_DGRAM, &addr_len) };
    if (server_fd == -1)
    {
        syslog(LOG_ERR, "Could not create server socket (%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char buf[buf_size];
    char addr_str[inet_sock_addr_str_len];
    while (true)
    {
        sockaddr_storage claddr;
        sockaddr *claddrp{ reinterpret_cast<sockaddr *>(&claddr) };
        socklen_t len{ sizeof(sockaddr_storage) };
        ssize_t num_read{ recvfrom(server_fd, buf, buf_size, 0, claddrp, &len) };
        if (num_read == -1)
        {
            syslog(LOG_ERR, "recvfrom (%d %s)", static_cast<int>(errno), strerror(errno));
            err_exit("recvfrom");
        }
        if (sendto(server_fd, buf, num_read, 0, claddrp, len) != num_read)
            syslog(LOG_WARNING, "Error echoing response to %s (%s)",
                inet_address_str(claddrp, len, addr_str, inet_sock_addr_str_len),
                strerror(errno));
    }

    return 0;
}

