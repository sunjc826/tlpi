#include "tlpi_hdr.h"
// setvbuf demo
void demo1()
{
    static constexpr int buf_size{ 1024 };
    static char buf[buf_size];

    if (std::setvbuf(stdout, buf, _IOFBF, buf_size) != 0)
        err_exit("setvbuf");
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

