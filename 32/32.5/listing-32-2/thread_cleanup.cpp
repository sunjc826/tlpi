#include "tlpi_hdr.h"
#include <pthread.h>

static pthread_cond_t cond PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx PTHREAD_MUTEX_INITIALIZER;
static int glob{};

static void cleanup_handler(void *arg)
{
    int s;

    std::printf("cleanup: freeing block at %p\n", arg);
    delete[] static_cast<char *>(arg);

    std::puts("cleanup: unlocking mutex\n");
    if ((s = pthread_mutex_unlock(&mtx)) != 0)
        err_exit(s, "pthread_mutex_unlock");
}

static void *thread_func(void *arg)
{
    int s;
    void *buf{ new char[0x10000] };
    std::printf("thread:  allocated memory at %p\n", buf);

    if ((s = pthread_mutex_lock(&mtx)) != 0)
        err_exit(s, "pthread_mutex_lock");

    pthread_cleanup_push(cleanup_handler, buf);

    while (glob == 0)
    {
        if ((s = pthread_cond_wait(&cond, &mtx)) != 0)
            err_exit(s, "pthread_cond_wait");
    }

    std::puts("thread:  condition wait loop completed");
    pthread_cleanup_pop(1);

    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t thr;
    void *res;
    int s;

    if ((s = pthread_create(&thr, nullptr, thread_func, nullptr)) != 0)
        err_exit(s, "pthread_create");

    sleep(2);

    if (argc == 1) // cancel
    {
        std::puts("main:    about to cancel thread");
        if ((s = pthread_cancel(thr)) != 0)
            err_exit(s, "pthread_cancel");
    }
    else // cond
    {
        std::puts("main:    about to signal condition variable");
        glob = 1;
        if ((s = pthread_cond_signal(&cond)) != 0)
            err_exit(s, "pthread_cond_signal");
    }

    if ((s = pthread_join(thr, &res)) != 0)
        err_exit(s, "pthread_join");

    if (res == PTHREAD_CANCELED)
        std::puts("main:    thread was cancelled");
    else
        std::puts("main:    thread was terminated normally");

    return 0;
}

