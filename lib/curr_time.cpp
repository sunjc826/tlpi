#include <ctime>
#include "curr_time.h"

constexpr int buf_size{ 1000 };

// Uses strftime, if format is nullptr, use %c as a specifier
// which gives the same output as ctime, but without the trailing
// newline. The returned string is statically allocated.
// Returns nullptr on error.
char *curr_time(const char *format)
{
    static char buf[buf_size];
    std::time_t t{ std::time(nullptr) };
    std::tm *lcp{ std::localtime(&t) };
    if (lcp == nullptr)
        return nullptr;

    std::size_t s{ std::strftime(buf, buf_size, format, lcp) };
    return (s == 0) ? nullptr : buf;
}
