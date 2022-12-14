#include "tlpi_hdr.h"
int main(int argc, char *argv[])
{
    if (argc != 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s pathname\n", argv[0]);

    const char *env_vec[]{ "GREET=salut", "BYE=adieu", nullptr };
    char *filename{ std::strrchr(argv[1], '/') };
    if (filename != nullptr)
        ++filename;
    else
        filename = argv[1];
    execle(argv[1], filename, "hello world", "goodbye", static_cast<char *>(nullptr), env_vec);
    err_exit("execle");

    return 0;
}
