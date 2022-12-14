#include "tlpi_hdr.h"
#include <pthread.h>

void join_with_no_check(pthread_t pthread_id)
{
    /*
        When pthread_id equals pthread_self(),
        this should result in either error:
        EDEADLK 35 Resource deadlock avoided
        EDEADLOCK 35 Resource deadlock avoided
        */
    int s{ pthread_join(pthread_id, nullptr) };
    if (s != 0)
        err_exit(s, "pthread_join");
}

void join_with_check(pthread_t pthread_id)
{
    if (pthread_equal(pthread_id, pthread_self()))
        return;
    join_with_no_check(pthread_id);
}

void *join_with_self(void *fptr)
{
    reinterpret_cast<void (*)(pthread_t)>(fptr)(pthread_self());
}


int main(int argc, char *argv[])
{
    pthread_t pthread_id;
    void(*fptr)(pthread_t) { argc > 1 ? join_with_check : join_with_no_check };
    int s;
    if ((s = pthread_create(&pthread_id, nullptr, join_with_self, reinterpret_cast<void *>(fptr))) != 0)
        err_exit(s, "pthread_create");
    std::puts("Before sleep");
    sleep(0);
    std::puts("After sleep");
    if ((s = pthread_join(pthread_id, nullptr)) != 0)
        err_exit(s, "pthread_join");
    std::puts("Joined successfully");
    return 0;
}
