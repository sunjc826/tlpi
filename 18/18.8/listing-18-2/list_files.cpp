#include "tlpi_hdr.h"
#include <dirent.h>

static void list_files(const char *dirpath)
{
    bool is_current{ std::strcmp(dirpath, ".") == 0 };
    DIR *dirp{ opendir(dirpath) };
    if (dirp == nullptr)
        err_msg("opendir failed on '%s'", dirpath);

    while (true)
    {
        errno = 0;
        dirent *entry{ readdir(dirp) };
        if (entry == nullptr)
            break;

        if (std::strcmp(entry->d_name, ".") == 0 || std::strcmp(entry->d_name, "..") == 0)
            continue;

        if (!is_current)
            printf("%s/", dirpath);
        printf("%s\n", entry->d_name);
    }

    if (errno != 0)
        err_exit("readdir");

    if (closedir(dirp) == -1)
        err_msg("closedir");
}

int main(int argc, char *argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0)
        usage_err("%s [dir...]\n", argv[0]);

    if (argc == 1)
        list_files(".");
    else
        for (int i{ 1 }; i < argc; ++i)
            list_files(argv[i]);

    return 0;
}

