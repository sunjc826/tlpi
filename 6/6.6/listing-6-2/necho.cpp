#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    for (int i{}; i < argc; ++i)
        std::printf("argv[%d] = %s\n", i, argv[i]);

    return 0;
}
