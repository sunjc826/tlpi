#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc < 4 || std::strcmp(argv[1], "--help") == -1)
        usage_err("%s pid num-sigs sig-num [sig-num-2]\n", argv[0]);

    long pid{ get_long(argv[1], 0, "PID") };
    int num_sigs{ get_int(argv[2], GN_GT_0, "num-sigs") };
    int sig{ get_int(argv[3], 0, "sig-num") };

    std::printf("%s: sending signal %d to process %ld %d times\n",
        argv[0], sig, pid, num_sigs);

    for (int i{ 0 }; i < num_sigs; ++i)
        if (kill(pid, sig) == -1)
            err_exit("kill");

    if (argc > 4)
    {
        int sig2{ get_int(argv[4], 0, "sig-num-2") };
        if (kill(pid, sig2) == -1)
            err_exit("kill");
    }
    std::printf("%s: exiting\n", argv[0]);

    return 0;
}
