#include "tlpi_hdr.h"

// for saved-set-UID process
void revoke_own_privilege()
{
    if (setuid(getuid()) == -1)
        err_exit("setuid");
}

// for saved-set-UID process
void revoke_and_reinstate_privilege()
{
    uid_t prev{ geteuid() };

    if (seteuid(getuid()) == -1)
        err_exit("seteuid");

    // no longer privileged

    if (seteuid(prev) == -1)
        err_exit("seteuid");
}

int main(int argc, char const *argv[])
{

    return 0;
}

