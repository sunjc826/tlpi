#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    const char *env_val;
    if ((env_val = getenv("SHELL")) != nullptr)
        std::puts(env_val);

    if ((env_val = getenv("PATH")) != nullptr)
        std::puts(env_val);
    return 0;
}
