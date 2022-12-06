#include "tlpi_hdr.h"
#include <clocale>
#include <ctime>
#include <sys/time.h>

constexpr double seconds_in_tropical_year{ 365.24219 * 24 * 60 * 60 };

int main(int argc, char const *argv[])
{
    std::time_t t{ std::time(nullptr) };
    std::printf("Seconds since the Epoch (1 Jan 1970): %ld", static_cast<long>(t));
    std::printf(" (about %6.3f years)\n", t / seconds_in_tropical_year);

    struct timeval tv;
    if (gettimeofday(&tv, nullptr) == -1)
        err_exit("gettimeofday");
    std::printf("gettimeofday() returned %ld secs, %ld microsecs\n", static_cast<long>(tv.tv_sec), static_cast<long>(tv.tv_usec));

    struct std::tm *gmp{ std::gmtime(&t) };
    if (gmp == nullptr)
        err_exit("gm_time");
    struct std::tm gm { *gmp };

    std::printf("Broken down by gmtime():\n");
    std::printf("year=%d mon=%d mday=%d hour=%d min=%d sec=%d",
        gm.tm_year, gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
    std::printf("wday=%d yday=%d isdst=%d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

    struct std::tm *lcp{ std::localtime(&t) };
    if (lcp == nullptr)
        err_exit("localtime");

    struct std::tm lc { *lcp };
    std::printf("Broken down by localtime():\n");
    std::printf("year=%d mon=%d mday=%d hour=%d min=%d sec=%d",
        lc.tm_year, lc.tm_mon, lc.tm_mday, lc.tm_hour, lc.tm_min, lc.tm_sec);
    std::printf("wday=%d yday=%d isdst=%d\n", lc.tm_wday, lc.tm_yday, lc.tm_isdst);

    std::printf("asctime() formats the gmtime() value as: %s", asctime(&gm));
    std::printf("asctime() formats the localtime() value as: %s", asctime(&lc));
    std::printf("ctime() formats the time() value as: %s", ctime(&t));

    std::printf("mktime() of gmtime() value: %ld secs\n", static_cast<long>(mktime(&gm)));
    std::printf("mktime() of localtime() value: %ld secs\n", static_cast<long>(mktime(&lc)));

    return 0;
}


