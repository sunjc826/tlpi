#include "tlpi_hdr.h"

void sysconf_print(const char *msg, int name)
{
    errno = 0;
    long lim{ sysconf(name) };
    if (lim != -1)
        std::printf("%s %ld\n", msg, lim);
    else
    {
        if (errno == 0)
            std::printf("%s (indeterminate)\n", msg);
        else
            err_exit("sysconf %s", msg);
    }
}

int main(int argc, char const *argv[])
{
    sysconf_print("_SC_ARG_MAX:", _SC_ARG_MAX);
    sysconf_print("_SC_LOGIN_NAME_MAX:", _SC_LOGIN_NAME_MAX);
    sysconf_print("_SC_OPEN_MAX:", _SC_OPEN_MAX);
    sysconf_print("_SC_NGROUPS_MAX:", _SC_NGROUPS_MAX);
    sysconf_print("_SC_PAGESIZE:", _SC_PAGESIZE);
    sysconf_print("_SC_RTSIG_MAX:", _SC_RTSIG_MAX);
    return 0;
}
