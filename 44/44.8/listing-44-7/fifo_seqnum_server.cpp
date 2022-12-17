#include "fifo_seqnum.h"
#include <signal.h>

int main(int argc, char *argv[])
{
    umask(0);
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR) == -1 && errno != EEXIST)
        err_exit("mkfifo %s", SERVER_FIFO);

    int server_fd{ open(SERVER_FIFO, O_RDONLY) };
    if (server_fd == -1)
        err_exit("open %s", SERVER_FIFO);

    int dummy_fd{ open(SERVER_FIFO, O_WRONLY) };
    if (dummy_fd == -1)
        err_exit("open %s", SERVER_FIFO);

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        err_exit("signal");

    int seq_num;
    while (true)
    {
        Request cur_request;
        if (read(server_fd, &cur_request, sizeof(cur_request)) != sizeof(cur_request))
        {
            std::fputs("Partial read/Error\n", stderr);
            continue;
        }

        char client_fifo[CLIENT_FIFO_NAME_LEN];
        if (snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, static_cast<long>(cur_request.pid)) == CLIENT_FIFO_NAME_LEN)
            client_fifo[CLIENT_FIFO_NAME_LEN - 1] = '\0';

        int client_fd{ open(client_fifo, O_WRONLY) };
        if (client_fd == -1)
        {
            err_msg("open %s", client_fifo);
            continue;
        }

        Response cur_response{ .seq_num{seq_num} };
        if (write(client_fd, &cur_response, sizeof(Response)) != sizeof(Response))
        {
            std::fprintf(stderr, "Error writing to FIFO %s\n", client_fifo);
            continue;
        }

        if (close(client_fd) == -1)
            err_msg("close");

        seq_num += cur_request.seq_len;
    }

    return 0;
}

