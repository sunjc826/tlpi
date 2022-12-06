#include "tlpi_hdr.h"
#include <ctime>
#include <clocale>

constexpr int buf_size{ 200 };

int main(int argc, char const *argv[])
{
    if (std::setlocale(LC_ALL, "") == nullptr)
        err_exit("setlocale");

    std::time_t t{ time(nullptr) };

    std::printf("ctime() of time() value is: %s", std::ctime(&t));

    std::tm *lcp{ std::localtime(&t) };
    if (lcp == nullptr)
        err_exit("localtime");
    std::printf("asctime() of time() value is %s", std::asctime(lcp));

    char buf[buf_size];
    if (std::strftime(buf, buf_size, "%A, %d %B %Y, %H:%M:%S %Z", lcp) == 0)
        fatal("strftime returned 0");

    std::printf("strftime() of local time is: %s\n", buf);

    return 0;
}

