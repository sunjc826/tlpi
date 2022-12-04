#include "tlpi_hdr.h"
#include "error_functions.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
    constexpr int max_read{ 20 };
    char buf[max_read + 1];
    int num_read{ static_cast<int>(read(STDIN_FILENO, buf, max_read)) };
    if (num_read == -1)
        err_exit("read");
    buf[num_read] = '\0';
    std::printf("%s\n", buf);

    return 0;
}
