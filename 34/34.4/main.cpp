#include "tlpi_hdr.h"
int main(int argc, char *argv[])
{
    std::cout << ctermid(nullptr) << '\n';
    return 0;
}
