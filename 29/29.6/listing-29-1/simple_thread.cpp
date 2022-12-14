#include "tlpi_hdr.h"
#include <pthread.h>

static void *thread_func(void *arg)
{
    // non-portable
    std::printf("Thread ID: %ld\n", static_cast<long>(pthread_self()));

    char *s{ static_cast<char *>(arg) };
    std::printf("%s", s);

    return reinterpret_cast<void *>(std::strlen(s));
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    void *res;
    int s{ pthread_create(&t1, nullptr, thread_func, const_cast<char *>("Hello world\n")) };
    if (s != 0)
        err_exit(s, "pthread_create");

    std::puts("Message from main()");
    s = pthread_join(t1, &res);
    if (s != 0)
        err_exit(s, "pthread_join");

    std::printf("Thread returned %ld\n", static_cast<long>(reinterpret_cast<size_t>(res)));

    return 0;
}

