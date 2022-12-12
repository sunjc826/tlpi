#include "tlpi_hdr.h"

void handler1(int sig)
{
    psignal(sig, "handler 1 received signal");
}

void flags_resethand()
{
    sigaction_st sa;
    sa.sa_handler = handler1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &sa, nullptr);
}

void handler2(int sig)
{
    static constexpr int total_times{ 5 };
    static int num_times{ total_times };
    psignal(sig, "handler 2 received signal");
    if (--num_times >= 0)
        raise(sig);
    else
        num_times = total_times;
    std::puts("handler2 done");
}

void flags_nodefer()
{
    sigaction_st sa;
    sa.sa_handler = handler2;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    sigaction(SIGINT, &sa, nullptr);
}


int main(int argc, char const *argv[])
{
    std::puts("Demo for when flags=SA_NODEFER");
    flags_nodefer();
    std::puts("SIGINT");
    raise(SIGINT);
    std::puts("SIGINT");
    raise(SIGINT);

    std::puts("Demo for when flags=SA_RESETHAND");
    flags_resethand();
    std::puts("SIGINT");
    raise(SIGINT);
    std::puts("SIGINT");
    raise(SIGINT);

    return 0;
}
