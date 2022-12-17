#define _DEFAULT_SOURCE
#include "fifo_seqnum.h"

static void remove_fifo(int status, void *arg)
{
    char *client_fifo{ static_cast<char *>(arg) };
    unlink(client_fifo);
}

int main(int argc, char *argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0)
        usage_err("%s [seq-len...]\n", argv[0]);

    char client_fifo[CLIENT_FIFO_NAME_LEN];
    if (on_exit(remove_fifo, client_fifo) != 0)
        err_exit("on_exit");
    pid_t own_pid{ getpid() };
    umask(0);
    snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, static_cast<long>(own_pid));
    if (mkfifo(client_fifo, S_IRUSR | S_IWUSR) == -1 && errno != EEXIST)
        err_exit("mkfifo %s", client_fifo);

    int server_fd{ open(SERVER_FIFO, O_WRONLY) };
    if (server_fd == -1)
        err_exit("open %s", SERVER_FIFO);

    Request request{ .pid{own_pid}, .seq_len{ argc > 1 ? get_int(argv[1], GN_GT_0, "seq-len") : 1 } };
    if (write(server_fd, &request, sizeof(Request)) != sizeof(Request))
        fatal("Partial write/Error");

    int client_fd{ open(client_fifo, O_RDONLY) };
    if (client_fd == -1)
        err_exit("open %s", client_fifo);

    Response response;
    if (read(client_fd, &response, sizeof(Response)) != sizeof(Response))
        fatal("Partial read/Error");

    std::printf("%d\n", response.seq_num);

    return 0;
}

