#include "tlpi_hdr.h"
#include <ftw.h>

static void usage_error(const char *prog_name, const char *msg)
{
    if (msg != nullptr)
        std::fprintf(stderr, "%s\n", msg);
    std::fprintf(stderr, "Usage: %s [-d] [-m] [-p] [directory-path]\n", prog_name);
    std::fprintf(stderr, "\t-d Use FTW_DEPTH flag\n");
    std::fprintf(stderr, "\t-m Use FTW_MOUNT flag\n");
    std::fprintf(stderr, "\t-p Use FTW_PHYS flag\n");
    std::exit(EXIT_FAILURE);
}

typedef struct stat stat_st;
static int dir_tree(const char *pathname, const stat_st *sbuf, int type, FTW *ftwb)
{
    switch (sbuf->st_mode & S_IFMT)
    {
    case S_IFREG: std::printf("-"); break;
    case S_IFDIR: std::printf("d"); break;
    case S_IFCHR: std::printf("c"); break;
    case S_IFBLK: std::printf("b"); break;
    case S_IFLNK: std::printf("l"); break;
    case S_IFIFO: std::printf("p"); break;
    case S_IFSOCK: std::printf("s"); break;
    default: std::printf("?"); break;
    }

    std::printf(" %s  ",
        (type == FTW_D) ? "D  " :
        (type == FTW_DNR) ? "DNR" :
        (type == FTW_DP) ? "DP " :
        (type == FTW_F) ? "F  " :
        (type == FTW_SL) ? "SL " :
        (type == FTW_SLN) ? "SLN" :
        (type == FTW_NS) ? "NS " : "   ");

    if (type != FTW_NS)
        std::printf("%7ld ", static_cast<long>(sbuf->st_ino));
    else
        std::printf("       ");

    std::printf(" %*s", 4 * ftwb->level, "");
    std::printf("%s\n", &pathname[ftwb->base]);
    return 0;
}

int main(int argc, char *argv[])
{
    int flags{}, opt{};
    while ((opt = getopt(argc, argv, "dmp")) != -1)
    {
        switch (static_cast<char>(opt))
        {
        case 'd': flags |= FTW_DEPTH; break;
        case 'm': flags |= FTW_MOUNT; break;
        case 'p': flags |= FTW_PHYS; break;
        default: usage_err(argv[0], nullptr); break;
        }
    }

    if (argc > optind + 1)
        usage_err(argv[0], nullptr);


    if (nftw((argc > optind) ? argv[optind] : ".", dir_tree, 10, flags) == -1)
    {
        std::perror("nftw");
        std::exit(EXIT_FAILURE);
    }

    return 0;
}

