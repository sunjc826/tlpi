#include <cstdio>
#include <cstdlib>

#define BSS

int main(int argc, char const *argv[])
{
    static int key{ 9973 };

    static char mbuf[10240000]{
    #ifdef BSS
        // This will result in a much smaller executable
    #else
        // This will result in the executable reaching about 10MB in size
        1,2,3
    #endif
    };

    char *p{ static_cast<char *>(std::malloc(1024)) };

    return 0;
}

