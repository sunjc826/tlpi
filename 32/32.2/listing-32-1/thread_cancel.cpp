#include "tlpi_hdr.h"
#include <pthread.h>

static void *thread_func(void *arg)
{
    std::puts("New thread started");
    for (int i{ 1 };;++i)
    {
        std::printf("Loop %d\n", i);
        sleep(1);
    }

    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t thr;
    int s;
    void *res;

    if ((s = pthread_create(&thr, nullptr, thread_func, nullptr)) != 0)
        err_exit(s, "pthread_create");

    sleep(3);

    if ((s = pthread_cancel(thr)) != 0)
        err_exit("pthread_cancel");

    if ((s = pthread_join(thr, &res)) != 0)
        err_exit("pthread_join");

    if (res == PTHREAD_CANCELED)
        std::puts("Thread was cancelled");
    else
        std::puts("Thread was not cancelled (should not happen!)");

    return 0;
}
