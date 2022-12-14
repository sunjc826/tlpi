#include "tlpi_hdr.h"
#include "print_wait_status.h"
#include <sys/wait.h>

constexpr int max_cmd_len{ 200 };

int main(int argc, char *argv[])
{
    char str[max_cmd_len];
    int status;
    while (true)
    {
        std::printf("Command: ");
        fflush(stdout);
        if (std::fgets(str, max_cmd_len, stdin) == nullptr)
            break;
        status = system(str);
        std::printf("system() returned: status=0x%04x (%d,%d)\n",
            static_cast<unsigned>(status), status >> 8, status & 0xff);
        if (status == -1)
            err_exit("system");
        else
        {
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
                std::printf("(Probably) could not invoke shell\n");
            else
                print_wait_status(nullptr, status);
        }
    }

    return 0;
}

