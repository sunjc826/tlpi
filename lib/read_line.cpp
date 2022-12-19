#include "read_line.h"
#include <unistd.h>
#include <errno.h>

ssize_t read_line(int fd, void *buffer, size_t n)
{
    if (n <= 0 || buffer == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    char *buf{ static_cast<char *>(buffer) };
    ssize_t tot_read{};
    while (true)
    {
        char ch;
        ssize_t num_read{ read(fd, &ch, 1) };

        if (num_read == -1)
        {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        else if (num_read == 0)
        {
            if (tot_read == 0)
                return 0;
            else
                break;
        }
        else // num_read equals 1
        {
            if (tot_read < n - 1)
            {
                ++tot_read;
                *buf++ = ch;
            }

            if (ch == '\n')
                break;
        }
    }

    *buf = '\0';
    return tot_read;
}
