#include "tlpi_hdr.h"
#include <pthread.h>

static int glob{};
static void *thread_func(void *arg)
{
    int loops{ *static_cast<int *>(arg) };
    int loc;
    for (int i{}; i < loops; ++i)
    {
        loc = glob;
        ++loc;
        glob = loc;
    }

    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops{ (argc > 1) ? get_int(argv[1], GN_GT_0, "num-loops") : 10000000 };
    int s;
    if ((s = pthread_create(&t1, nullptr, thread_func, &loops)) != 0)
        err_exit(s, "pthread_create");

    if ((s = pthread_create(&t2, nullptr, thread_func, &loops)) != 0)
        err_exit(s, "pthread_create");

    if ((s = pthread_join(t1, nullptr)) != 0)
        err_exit(s, "pthread_join");

    if ((s = pthread_join(t2, nullptr)) != 0)
        err_exit(s, "pthread_join");

    std::printf("glob = %d\n", glob);

    return 0;
}

