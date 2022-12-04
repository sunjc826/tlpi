#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>
int main(int argc, char const *argv[])
{
    std::printf("%d\n", errno);
    std::perror("");
    std::string s{ std::strerror(errno) };
    std::puts(s.c_str());
    return 0;
}

