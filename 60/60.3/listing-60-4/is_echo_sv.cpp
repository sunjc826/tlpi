#include <signal.h>
#include <syslog.h>
#include <sys/wait.h>
#include "become_daemon.h"
#include "inet_sockets.h"
#include "tlpi_hdr.h"

constexpr const char *service{ "echo" };
constexpr size_t buf_size{ 4096lu };

static void grim_reaper(int sig)
{
    int saved_errno{ errno };
    while (waitpid(-1, nullptr, WNOHANG) > 0)
        continue;
    errno = saved_errno;
}

static void handle_request(int connected_fd)
{
    char buf[buf_size];
    ssize_t num_read;
    while ((num_read = read(connected_fd, buf, buf_size)) > 0)
    {
        if (write(connected_fd, buf, num_read) != num_read)
        {
            syslog(LOG_ERR, "write() failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if (num_read == -1)
    {
        syslog(LOG_ERR, "Error from read(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[])
{
    if (become_daemon(0) == -1)
        err_exit("become_daemon");

    sigaction_st sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grim_reaper;
    if (sigaction(SIGCHLD, &sa, nullptr) == -1)
    {
        syslog(LOG_ERR, "Error from sigaction(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int listening_fd{ inet_listen(service, 10, nullptr) };
    if (listening_fd == -1)
    {
        syslog(LOG_ERR, "Could not create server socket (%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        int connection_fd{ accept(listening_fd, nullptr, nullptr) };
        if (connection_fd == -1)
        {
            syslog(LOG_ERR, "Failure in accept(): %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        switch (fork())
        {
        case -1:
            syslog(LOG_ERR, "Can't create child (%s)", strerror(errno));
            close(connection_fd);
            break;
        case 0:
            close(listening_fd);
            handle_request(connection_fd);
            _exit(EXIT_SUCCESS);
            break;
        default:
            close(connection_fd);
            break;
        }
    }

    return 0;
}

