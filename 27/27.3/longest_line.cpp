#include "tlpi_hdr.h"
int main(int argc, char *argv[])
{
    execl("27/27.3/longest_line.awk", "27/27.3/longest_line.awk", "tmnt.txt", nullptr);
    err_exit("execl");
    return 0;
}
