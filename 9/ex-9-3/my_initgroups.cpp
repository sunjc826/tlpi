#include "tlpi_hdr.h"
#include <grp.h>
#include <climits>
#include <vector>

constexpr int group_array_len{ NGROUPS_MAX + 1 };

int my_initgroups(const char *username, gid_t group_id)
{
    group *entry;
    std::vector<gid_t> grouplist;
    grouplist.reserve(group_array_len);
    errno = 0;
    while ((entry = getgrent()) != nullptr)
    {
        for (char **member{ entry->gr_mem }; *member != nullptr; ++member)
        {
            if (strcmp(*member, username) == 0)
                grouplist.push_back(entry->gr_gid);
        }
    }
    if (errno != 0)
        return -1;

    grouplist.push_back(group_id);

    for (gid_t id : grouplist)
        std::cout << id << ' ';
    std::cout << '\n';

    return setgroups(grouplist.size(), grouplist.data());
}


int main(int argc, char const *argv[])
{
    if (argc < 3 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s <username> <groupid>\n", argv[0]);

    if (my_initgroups(argv[1], static_cast<gid_t>(get_long(argv[2], GN_ANY_BASE, "group_id"))))
        err_exit("setgroups");

    char **next_argv{ nullptr };
    execv("idshow", next_argv);
    err_exit("execv");
    return 0;
}
