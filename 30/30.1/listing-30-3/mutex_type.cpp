#include "tlpi_hdr.h"
#include <pthread.h>

int main(int argc, char const *argv[])
{
    pthread_mutex_t mtx;
    pthread_mutexattr_t mtx_attr;
    int s;

    if ((s = pthread_mutexattr_init(&mtx_attr)) != 0)
        err_exit(s, "pthread_mutexattr_init");

    if ((s = pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_ERRORCHECK)) != 0)
        err_exit(s, "pthread_mutexattr_settype");

    if ((s = pthread_mutex_init(&mtx, &mtx_attr)) != 0)
        err_exit(s, "pthread_mutex_init");

    if ((s = pthread_mutexattr_destroy(&mtx_attr)) != 0)
        err_exit(s, "pthread_mutexattr_destroy");

    if ((s = pthread_mutex_destroy(&mtx)) != 0)
        err_exit(s, "pthread_mutex_destroy");

    return 0;
}

