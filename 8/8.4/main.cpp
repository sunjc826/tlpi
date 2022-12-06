#include "tlpi_hdr.h"
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <cstdio>

void scan_passwd_file()
{
    passwd *pwd;
    while ((pwd = getpwent()) != nullptr)
        std::printf("%-8s %5ld\n", pwd->pw_name, static_cast<long>(pwd->pw_uid));
    endpwent();
}

void scan_group_file()
{
    group *grp;
    while ((grp = getgrent()) != nullptr)
        std::printf("%-8s %5ld\n", grp->gr_name, static_cast<long>(grp->gr_gid));

    endgrent();
}

int main(int argc, char const *argv[])
{
    std::puts("passwd");
    scan_passwd_file();
    std::putchar('\n');
    std::puts("group");
    scan_group_file();
    return 0;
}
