#include "tlpi_hdr.h"
#include <signal.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
        usage_err("%s <signum>\n", program_invocation_name);
    int signum{ get_int(argv[1], 0, "signum") };

    char *sigstr{ strsignal(signum) };
    std::printf("%s\n", sigstr);
    psignal(signum, "This is a signal");

    return 0;
}
