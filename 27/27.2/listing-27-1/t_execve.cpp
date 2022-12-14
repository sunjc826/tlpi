#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || std::strcmp(argv[1], "--help") == -1)
        usage_err("%s pathname\n", argv[0]);

    char *arg_vec[10];
    char *env_vec[]{ "GREET=salut", "BYE=adieu", nullptr };

    arg_vec[0] = std::strrchr(argv[1], '/');
    // get basename
    if (arg_vec[0] != nullptr)
        ++arg_vec[0];
    else
        arg_vec[0] = argv[1];

    arg_vec[1] = "hello world";
    arg_vec[2] = "goodbye";
    arg_vec[3] = nullptr;

    execve(argv[1], arg_vec, env_vec);
    err_exit("execve");

    return 0;
}
