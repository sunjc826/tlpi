#include "tlpi_hdr.h"

void noflush_demo()
{
    std::puts("No flush");
    fflush(stdout);
    std::printf("To man the world is twofold, ");
    write(STDOUT_FILENO, "in accordance with his twofold attitude.\n", 41);
}

void with_flush_demo()
{
    std::puts("With flush");
    fflush(stdout);
    std::printf("To man the world is twofold, ");
    std::fflush(stdout);
    write(STDOUT_FILENO, "in accordance with his twofold attitude.\n", 41);
}

int main(int argc, char const *argv[])
{
    noflush_demo();
    putchar('\n');
    fflush(stdout);
    with_flush_demo();
    return 0;
}
