#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <limits>
#include "get_num.h"
static void gn_fail(const char *fname, const char *msg, const char *arg, const char *name)
{
    std::fprintf(stderr, "%s error", fname);
    if (name != nullptr)
        std::fprintf(stderr, " (in %s)", name);
    std::fprintf(stderr, ": %s\n", msg);
    if (arg != nullptr && *arg != '\0')
        std::fprintf(stderr, "        offending text: %s\n", arg);

    std::exit(EXIT_FAILURE);
}

static long get_num(const char *fname, const char *arg, int flags, const char *name)
{
    if (arg == nullptr || *arg == '\0')
        gn_fail(fname, "null or empty string", arg, name);

    int base{ (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 : (flags & GN_BASE_16) ? 16 : 10 };

    errno = 0;
    char *endptr;
    long res{ strtol(arg, &endptr, base) };

    if (errno != 0)
        gn_fail(fname, "strtol() failed", arg, name);

    if (*endptr != '\0')
        gn_fail(fname, "nonnumeric characters", arg, name);

    if ((flags & GN_NONNEG) && res < 0)
        gn_fail(fname, "negative value not allowed", arg, name);

    if ((flags & GN_GT_0) && res <= 0)
        gn_fail(fname, "value must be > 0", arg, name);

    return res;
}

int get_int(const char *arg, int flags, const char *name)
{
    long res{ get_num("get_int", arg, flags, name) };
    if (res < INT_MIN || res > INT_MAX)
        gn_fail("get_int", "integer out of range", arg, name);

    return static_cast<int>(res);
}

long get_long(const char *arg, int flags, const char *name)
{
    return get_num("get_long", arg, flags, name);
}

