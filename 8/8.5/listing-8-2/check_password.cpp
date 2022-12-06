#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "tlpi_hdr.h"
#include <climits>
#include <pwd.h>
#include <shadow.h>

int main(int argc, char const *argv[])
{
    long lnmax{ sysconf(_SC_LOGIN_NAME_MAX) };
    if (lnmax == -1)
        lnmax = 256;

    char *username{ new char[lnmax] };

    std::printf("Username: ");
    std::fflush(stdout);
    if (std::fgets(username, lnmax, stdin) == nullptr)
        std::exit(EXIT_FAILURE);
    size_t len{ strlen(username) };
    if (username[len - 1] == '\n')
        username[len - 1] = '\0';
    passwd *pwd{ getpwnam(username) };
    if (pwd == nullptr)
        fatal("couldn't get password record");
    errno = 0;
    spwd *sp{ getspnam(username) };
    // On Ubuntu, use "sudo su" to enter root user
    if (sp == nullptr && errno == EACCES)
        fatal("no permission to read shadow password file");

    if (sp != nullptr)
        pwd->pw_passwd = sp->sp_pwdp;

    char *password{ getpass("Password: ") };
    char *encrypted{ crypt(password, pwd->pw_passwd) };
    for (char *p{ password }; *p != '\0'; ++p)
        *p = '\0';
    if (encrypted == nullptr)
        err_exit("crypt");

    bool auth_ok{ std::strcmp(encrypted, pwd->pw_passwd) == 0 };
    if (!auth_ok)
    {
        std::printf("Incorrect password\n");
        std::exit(EXIT_FAILURE);
    }

    std::printf("Successfully authenticated: UID=%ld\n", static_cast<long>(pwd->pw_uid));

    /* Do authenticated work */

    return 0;
}

