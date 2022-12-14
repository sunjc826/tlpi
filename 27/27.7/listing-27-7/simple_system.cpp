#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int simple_system(const char *command)
{
    pid_t child_pid{ fork() };
    switch (child_pid)
    {
    case -1:
        return -1;
    case 0:
        execl("sh", "sh", "-c", command, nullptr);
        _exit(127);
    default:
        if (int status; waitpid(child_pid, &status, 0) == -1)
            return -1;
        else
            return status;
    }
}
