#include "tlpi_hdr.h"
#include <pthread.h>

static pthread_cond_t thread_died PTHREAD_COND_INITIALIZER;
static pthread_mutex_t thread_mutex PTHREAD_MUTEX_INITIALIZER;

static int total_threads{};
static int num_live{};

static int num_unjoined{};

enum class ThreadState
{
    alive,
    terminated,
    joined,
};

struct ThreadInfo
{
    pthread_t tid;
    ThreadState state;
    int sleep_time;
};

static ThreadInfo *thread;

void *thread_func(void *arg)
{
    int idx{ static_cast<int>(reinterpret_cast<uintptr_t>(arg)) };
    int s;
    sleep(thread[idx].sleep_time);
    std::printf("Thread %d terminating\n", idx);

    if ((s = pthread_mutex_lock(&thread_mutex)) != 0)
        err_exit(s, "pthread_mutex_lock");

    ++num_unjoined;
    thread[idx].state = ThreadState::terminated;

    if ((s = pthread_mutex_unlock(&thread_mutex)) != 0)
        err_exit(s, "pthread_mutex_unlock");

    if ((s = pthread_cond_signal(&thread_died)) != 0)
        err_exit(s, "pthread_cond_signal");
    return nullptr;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s nsecs...\n", argv[0]);

    num_live = total_threads = argc - 1;

    if ((thread = new ThreadInfo[total_threads]) == nullptr)
        err_exit("new");

    int s;
    for (int idx{}; idx < total_threads; ++idx)
    {
        thread[idx].sleep_time = get_int(argv[idx + 1], GN_NONNEG, nullptr);
        thread[idx].state = ThreadState::alive;
        if ((s = pthread_create(&thread[idx].tid, nullptr, thread_func, reinterpret_cast<void *>(static_cast<uintptr_t>(idx)))) != 0)
            err_exit(s, "pthread_create");
    }

    while (num_live > 0)
    {
        if ((s = pthread_mutex_lock(&thread_mutex)) != 0)
            err_exit(s, "pthread_mutex_lock");

        while (num_unjoined == 0)
        {
            if ((s = pthread_cond_wait(&thread_died, &thread_mutex)) != 0)
                err_exit(s, "pthread_cond_wait");
        }

        for (int idx{}; idx < total_threads; ++idx)
        {
            if (thread[idx].state != ThreadState::terminated)
                continue;

            if ((s = pthread_join(thread[idx].tid, nullptr)) != 0)
                err_exit(s, "pthread_join");

            thread[idx].state = ThreadState::joined;
            --num_live;
            --num_unjoined;

            std::printf("Reaped thread %d (num_live=%d)\n", idx, num_live);
        }

        if ((s = pthread_mutex_unlock(&thread_mutex)) != 0)
            err_exit(s, "pthread_mutex_unlock");
    }

    return 0;
}

