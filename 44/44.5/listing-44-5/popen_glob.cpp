#include "tlpi_hdr.h"
#include "print_wait_status.h"
#include <cctype>
#include <climits>

#define POPEN_FMT "ls -d %s 2> /dev/null"
#define PAT_SIZE 50
#define PCMD_BUF_SIZE (sizeof(POPEN_FMT) + PAT_SIZE)

int main(int argc, char *argv[])
{
    char pat[PAT_SIZE];
    char popen_cmd[PCMD_BUF_SIZE];
    char pathname[PATH_MAX];
    while (true)
    {
        std::printf("pattern: ");
        std::fflush(stdout);
        if (std::fgets(pat, PAT_SIZE, stdin) == nullptr)
            break;
        size_t len{ std::strlen(pat) };
        if (len <= 1)
            continue;

        if (pat[len - 1] == '\n')
            pat[len - 1] = '\0';

        bool bad_pattern{};
        char bad_pattern_char;
        for (auto ch : pat)
        {
            if (!isalnum(ch) && strchr("_*?[^-].", ch) == nullptr)
            {
                bad_pattern = true;
                bad_pattern_char = ch;
                break;
            }

            if (ch == '\0')
                break;
        }

        if (bad_pattern)
        {
            std::printf("Bad pattern character: %c\n", bad_pattern_char);
            continue;
        }

        std::snprintf(popen_cmd, PCMD_BUF_SIZE, POPEN_FMT, pat);
        popen_cmd[PCMD_BUF_SIZE - 1] = '\0';

        FILE *fp{ popen(popen_cmd, "r") };
        if (fp == nullptr)
        {
            std::puts("popen() failed");
            continue;
        }

        int file_count{};
        while (std::fgets(pathname, PATH_MAX, fp) != nullptr)
        {
            std::printf("%s", pathname);
            ++file_count;
        }

        int status{ pclose(fp) };
        std::printf("    %d matching file%s\n", file_count, (file_count != 1) ? "s" : "");
        std::printf("    pclose() status == %#x\n", static_cast<unsigned>(status));
        if (status != -1)
            print_wait_status("\t", status);
    }

    return 0;
}

