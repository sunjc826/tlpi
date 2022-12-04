#include <cstdio>
#include <iterator>
#include <gnu/libc-version.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    std::puts("glibc version");
    std::printf("%d.%d\n", __GLIBC__, __GLIBC_MINOR__);
    std::printf("%s\n", gnu_get_libc_version());
    char config_val[100];
    confstr(_CS_GNU_LIBC_VERSION, config_val, std::size(config_val));
    std::printf("%s\n", config_val);
    return 0;
}
