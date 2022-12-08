#include "tlpi_hdr.h"
#include <libgen.h>

int main(int argc, char *argv[])
{
    for (int i{ 1 }; i < argc; ++i)
    {
        char *t1{ strdup(argv[i]) };
        if (t1 == nullptr)
            err_exit("strdup");

        char *t2{ strdup(argv[i]) };
        if (t2 == nullptr)
            err_exit("strdup");

        std::printf("%s ==> %s + %s\n", argv[i], dirname(t1), basename(t2));
        std::free(t1);
        std::free(t2);
    }

    return 0;
}

