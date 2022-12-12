// #define _XOPEN_SOURCE 600
#include <tlpi_hdr.h>

static char *str2;
static int handled{ 0 };
static constexpr char *salt{ "xx" };

static void handler(int sig)
{
    crypt(str2, salt);
    ++handled;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        usage_err("%s str1 str2\n", argv[0]);

    char *str1{ argv[1] };
    str2 = argv[2];
    char *cr1{ strdup(crypt(str1, salt)) };
    if (cr1 == nullptr)
        err_exit("strdup");

    typedef struct sigaction sigaction_st;
    sigaction_st sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigaction(SIGINT, &sa, nullptr) == -1)
        err_exit("sigaction");

    for (int call_num{ 0 }, mismatch{ 0 }; ; ++call_num)
    {
        if (std::strcmp(crypt(argv[1], salt), cr1) != 0)
        {
            ++mismatch;
            std::printf("Mismatch on call %d (mismatch=%d handled=%d\n", call_num, mismatch, handled);
        }
    }

    return 0;
}

