#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/acct.h>
#include <climits>

constexpr int time_buf_size{ 100 };

static long long comp_t_to_ll(comp_t ct)
{
    constexpr int exp_size{ 3 };
    constexpr int mantissa_size{ 13 };
    constexpr int mantissa_mask{ (1 << mantissa_size) - 1 };
    long long
        exp{ (ct >> mantissa_size) & ((1 << exp_size) - 1) },
        mantissa{ ct & mantissa_mask };
    return mantissa << (exp * 3);
}



typedef struct acct acct_st;

void print(int acct_fd)
{
    std::puts("command flags    term. user     start time           CPU     elapsed");
    std::puts("                status                               time    time");

    int num_read;
    acct_st ac;

    while ((num_read = read(acct_fd, &ac, sizeof(acct_st))) > 0)
    {
        if (num_read != sizeof(acct_st))
            fatal("partial read");

        std::printf("%-8.8s  ", ac.ac_comm);
        std::printf("%c", (ac.ac_flag & AFORK) ? 'F' : '-');
        std::printf("%c", (ac.ac_flag & ASU) ? 'S' : '-');
        std::printf("%c", (ac.ac_flag & AXSIG) ? 'X' : '-');
        std::printf("%c", (ac.ac_flag & ACORE) ? 'C' : '-');
#ifdef __linux__
        std::printf(" %#6lx   ", static_cast<unsigned long>(ac.ac_exitcode));
#else
        std::printf(" %#6lx   ", static_cast<unsigned long>(ac.ac_stat));
#endif
        char *s{ user_name_from_id(ac.ac_uid) };
        std::printf("%-8.8s ", (s == nullptr) ? "???" : s);

        time_t t{ ac.ac_btime };
        tm *loc{ localtime(&t) };
        if (loc == nullptr)
            std::printf("???Unknown time???  ");
        else
        {
            char time_buf[time_buf_size];
            strftime(time_buf, time_buf_size, "%Y-%m-%d %T ", loc);
            std::printf("%s ", time_buf);
        }

        std::printf("%5.2f %7.2f ",
            static_cast<double>((comp_t_to_ll(ac.ac_utime) + comp_t_to_ll(ac.ac_stime)) / sysconf(_SC_CLK_TCK)),
            static_cast<double>(comp_t_to_ll(ac.ac_etime) / sysconf(_SC_CLK_TCK)));

        std::putchar('\n');
    }

    if (num_read == -1)
        err_exit("read");
}

void print_v3(int acct_fd)
{
    std::puts("ver. command   flags   term.    PID   PPID  user    group      start date+time      CPU    elapsed");
    ssize_t num_read;
    acct_v3 ac;
    while ((num_read = read(acct_fd, &ac, sizeof(acct_v3))) > 0)
    {
        if (num_read != sizeof(acct_v3))
            fatal("partial read");

        std::printf("%1d   ", static_cast<int>(ac.ac_version));
        std::printf("%-8.8s  ", ac.ac_comm);
        std::printf("%c", (ac.ac_flag & AFORK) ? 'F' : '-');
        std::printf("%c", (ac.ac_flag & ASU) ? 'S' : '-');
        std::printf("%c", (ac.ac_flag & AXSIG) ? 'X' : '-');
        std::printf("%c", (ac.ac_flag & ACORE) ? 'C' : '-');

        std::printf(" %#6lx  ", static_cast<unsigned long>(ac.ac_exitcode));
        std::printf(" %5ld %5ld  ", static_cast<long>(ac.ac_pid), static_cast<long>(ac.ac_ppid));

        char *user_name{ user_name_from_id(ac.ac_uid) };
        std::printf("%-8.8s ", (user_name == nullptr) ? "???" : user_name);
        char *group_name{ group_name_from_id(ac.ac_gid) };
        std::printf("%-8.8s", (group_name == nullptr) ? "???" : group_name);

        time_t t{ ac.ac_btime };
        tm *loc{ localtime(&t) };
        if (loc == nullptr)
            std::printf("???Unknown time???  ");
        else
        {
            char time_buf[time_buf_size];
            strftime(time_buf, time_buf_size, "%Y-%m-%d %T ", loc);
            std::printf("%s ", time_buf);
        }

        std::printf("%5.2f %7.2f ",
            static_cast<double>((comp_t_to_ll(ac.ac_utime) + comp_t_to_ll(ac.ac_stime))) / sysconf(_SC_CLK_TCK),
            static_cast<double>(ac.ac_etime) / sysconf(_SC_CLK_TCK)
        );

        std::putchar('\n');
    }

    if (num_read == -1)
        err_exit("read");
}

int main(int argc, char const *argv[])
{
    if (argc != 2 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s file\n", argv[0]);

    int acct_fd{ open(argv[1], O_RDONLY) };
    if (acct_fd == -1)
        err_exit("open");

    print_v3(acct_fd);

    return 0;
}

