#include "tlpi_hdr.h"


void demo1()
{
    signal(SIGINT, SIG_IGN);
}

void custom_handler(int)
{
    std::puts("Custom handler called");
}

void demo2()
{
    signal(SIGINT, custom_handler);
}


// block SIGINT, then ignore SIGINT, then unblock SIGINT
int main(int argc, char const *argv[])
{
    print_pending_sigs(stdout, "1");
    sigset_t blockset;
    sigemptyset(&blockset);
    sigaddset(&blockset, SIGINT);
    sigprocmask(SIG_BLOCK, &blockset, nullptr);
    raise(SIGINT);

    print_pending_sigs(stdout, "2");


    demo2();
    print_pending_sigs(stdout, "3");


    sigprocmask(SIG_UNBLOCK, &blockset, nullptr);
    print_pending_sigs(stdout, "4");

    return 0;
}
