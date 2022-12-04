#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

off_t get_file_size(const char *pathname)
{
    int fd{ open(pathname, O_RDONLY) };
    return lseek(fd, 0, SEEK_END); // seek to the byte after the last byte of the file
}

void demo1()
{
    off_t file_size{ get_file_size("./main.cpp") };
    std::printf("%jd\n", static_cast<std::intmax_t>(file_size));
}

void demo2()
{

}

int main(int argc, char const *argv[])
{
    demo1();
    return 0;
}
