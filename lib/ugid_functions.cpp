#include "ugid_functions.h"
#include <pwd.h>
#include <grp.h>
#include <cstdlib>

char *user_name_from_id(uid_t uid)
{
    passwd *pwd{ getpwuid(uid) };
    return (pwd == nullptr) ? nullptr : pwd->pw_name;
}

uid_t user_id_from_name(const char *name)
{
    if (name == nullptr || *name == '\0')
        return static_cast<uid_t>(-1);
    char *endptr;
    uid_t u{ static_cast<gid_t>(std::strtol(name, &endptr, 10)) };
    if (*endptr == '\0')
        return u;

    passwd *pwd{ getpwnam(name) };
    return (pwd == nullptr) ? static_cast<uid_t>(-1) : pwd->pw_uid;
}

char *group_name_from_id(gid_t gid)
{
    group *grp{ getgrgid(gid) };
    return (grp == nullptr) ? nullptr : grp->gr_name;
}

gid_t group_id_from_name(const char *name)
{
    if (name == nullptr || *name == '\0')
        return static_cast<uid_t>(-1);

    char *endptr;
    gid_t g{ static_cast<gid_t>(std::strtol(name, &endptr, 10)) };
    if (*endptr == '\0')
        return g;

    group *grp{ getgrnam(name) };
    return (grp == nullptr) ? static_cast<uid_t>(-1) : grp->gr_gid;
}

