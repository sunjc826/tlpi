#include "tlpi_hdr.h"
#include <ctime>
int main(int argc, char const *argv[])
{
    std::puts("All time functions using the system clock will be limited in resolution by the kernel config CONFIG_HZ");
    std::printf("frequency unit of times(), _SC_CLK_TCK: %d\n", sysconf(_SC_CLK_TCK));
    std::printf("frequency unit of clock(), CLOCKS_PER_SEC: %d\n", CLOCKS_PER_SEC);
    return 0;
}
