#include "tlpi_hdr.h"
#include <sys/fsuid.h>
#include <climits>

constexpr int sg_size{ NGROUPS_MAX + 1 };

int main(int argc, char const *argv[])
{
    uid_t ruid, euid, suid;
    if (getresuid(&ruid, &euid, &suid) == -1)
        err_exit("getresuid");

    gid_t rgid, egid, sgid;
    if (getresgid(&rgid, &egid, &sgid) == -1)
        err_exit("getresgid");

    int fsuid{ setfsuid(0) };
    int fsgid{ setfsgid(0) };

    char *p;
    std::printf("UID: ");
    p = user_name_from_id(ruid);
    std::printf("real=%s (%ld); ", (p == nullptr) ? "???" : p, static_cast<long>(ruid));
    p = user_name_from_id(euid);
    std::printf("eff=%s (%ld); ", (p == nullptr) ? "???" : p, static_cast<long>(euid));
    p = user_name_from_id(suid);
    std::printf("saved=%s (%ld); ", (p == nullptr) ? "???" : p, static_cast<long>(suid));
    p = user_name_from_id(fsuid);
    std::printf("fs=%s (%ld); ", (p == nullptr) ? "???" : p, static_cast<long>(fsuid));
    std::putchar('\n');

    std::printf("GID: ");
    p = group_name_from_id(rgid);
    std::printf("real=%s (%ld); ", (p == nullptr) ? "???" : p, static_cast<long>(rgid));
    p = group_name_from_id(egid);
    std::printf("eff=%s (%ld); ", (p == nullptr) ? "???" : p, static_cast<long>(egid));
    p = group_name_from_id(sgid);
    std::printf("saved=%s (%ld); ", (p == nullptr) ? "???" : p, static_cast<long>(sgid));
    p = group_name_from_id(fsgid);
    std::printf("fs=%s (%ld); ", (p == nullptr) ? "???" : p, static_cast<long>(fsgid));
    std::putchar('\n');

    gid_t supplementary_groups[sg_size];
    int num_groups{ getgroups(sg_size, supplementary_groups) };
    if (num_groups == -1)
        err_exit("getgroups");

    std::printf("Supplementary groups (%d): ", num_groups);
    for (int i{ 0 }; i < num_groups; ++i)
    {
        p = group_name_from_id(supplementary_groups[i]);
        std::printf("%s (%ld) ", (p == nullptr) ? "???" : p, static_cast<long>(supplementary_groups[i]));
    }
    std::putchar('\n');

    return 0;
}
