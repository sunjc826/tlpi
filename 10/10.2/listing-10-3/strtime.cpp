#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#include "tlpi_hdr.h"
#include <ctime>
#include <clocale>

constexpr int sbuf_size{ 1000 };

int main(int argc, char const *argv[])
{
    if (argc < 3 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s input-date-time in-format [out-format]\n", argv[0]);

    if (std::setlocale(LC_ALL, "") == nullptr)
        err_exit("setlocale");

    std::tm lc{};
    if (strptime(argv[1], argv[2], &lc) == nullptr)
        fatal("strptime");

    lc.tm_isdst = -1;

    std::printf("calendar time (seconds since Epoch): %ld\n", static_cast<long>(std::mktime(&lc)));

    const char *output_format{ (argc > 3) ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z" };
    char sbuf[sbuf_size];
    if (strftime(sbuf, sbuf_size, output_format, &lc) == 0)
        fatal("strftime returned 0");

    std::printf("strftime() yields: %s\n", sbuf);

    return 0;
}

