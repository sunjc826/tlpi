#include "tlpi_hdr.h"
#include <pwd.h>
#include <cassert>

passwd *my_getpwnam(const char *name)
{
    passwd *entry;

    while (errno = 0, (entry = getpwent()) != nullptr)
        if (std::strcmp(name, entry->pw_name) == 0)
            return entry;
    if (errno != 0)
        err_exit("getpwent");
    return nullptr;
}

int main(int argc, char const *argv[])
{
    passwd *entry{ my_getpwnam("root") };
    assert(entry != nullptr);
    std::printf("UID=%ld\n", static_cast<long>(entry->pw_uid));
    return 0;
}
