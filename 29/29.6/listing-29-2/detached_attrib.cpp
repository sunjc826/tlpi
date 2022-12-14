#include "tlpi_hdr.h"
#include <pthread.h>

void *thread_func(void *arg)
{
}

int main(int argc, char const *argv[])
{
    pthread_t thr;
    pthread_attr_t attr;
    int s;
    s = pthread_attr_init(&attr);
    if (s != 0)
        err_exit(s, "pthread_attr_init");

    s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (s != 0)
        err_exit(s, "pthread_attr_setdetachstate");

    s = pthread_create(&thr, &attr, thread_func, reinterpret_cast<void *>(1));
    if (s != 0)
        err_exit(s, "pthread_create");

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        err_exit(s, "pthread_attr_destroy");

    return 0;
}
