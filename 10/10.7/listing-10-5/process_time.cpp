#include "tlpi_hdr.h"
#include <sys/times.h>
#include <ctime>

static void diplay_process_times(const char *msg)
{
    static long clock_ticks{ ([]()->long {
        long conf{sysconf(_SC_CLK_TCK)};
        if (conf == -1)
            err_exit("sysconf");
        return conf;
    })() };

    if (msg != nullptr)
        std::printf("%s", msg);

    std::clock_t clock_time{ std::clock() };
    if (clock_time == -1)
        err_exit("clock");

    std::printf("\tclock() returns: %ld clocks-per-sec (%.2f secs)\n",
        static_cast<long>(clock_time), static_cast<double>(clock_time) / CLOCKS_PER_SEC);

    tms t;
    if (times(&t) == -1)
        err_exit("times");
    std::printf("\ttimes() yields: user CPU=%.2f; system CPU=%.2f\n",
        static_cast<double>(t.tms_utime) / clock_ticks, static_cast<double>(t.tms_stime) / clock_ticks);
}

int main(int argc, char const *argv[])
{
    std::printf("CLOCKS_ER_SEC=%ld  sysconf(_SC_CLK_TCK=%ld\n",
        static_cast<long>(CLOCKS_PER_SEC), sysconf(_SC_CLK_TCK));

    diplay_process_times("At program start:\n");
    int num_calls{ (argc > 1) ? get_int(argv[1], GN_GT_0, "num_calls") : 100000000 };
    for (int j{ 0 }; j < num_calls; ++j)
        static_cast<void>(getppid());

    diplay_process_times("After getppid() loop:\n");

    return 0;
}


