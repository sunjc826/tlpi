#include "tlpi_hdr.h"
#include "become_daemon.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdarg>
static constexpr const char *log_file{ "/tmp/ds.log" };
static constexpr const char *config_file{ "/tmp/ds.conf" };

int log_fd{ -1 };

void log_message(const char *format, ...)
{
    constexpr size_t buf_size{ 1000lu };
    char str[buf_size];
    va_list va;
    va_start(va, format);
    int num_written{ std::vsnprintf(str, buf_size, format, va) };
    if (num_written <= 0)
        fatal("vsnprintf");
    // Not needed if we are just using "write"
    // str[buf_size - 1] = '\0'; 
    va_end(va);
    write(log_fd, str, num_written);
}

void log_open(const char *file_name)
{
    if ((log_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
        err_exit("open");

    log_message("Opened log file\n");
}

void log_close()
{
    if (log_fd > 0)
        if (close(log_fd) == -1)
            err_exit("close");
}

void read_config_file(const char *file_name)
{
    int config_fd{ open(file_name, O_RDONLY) };
    if (config_fd == -1)
        err_exit("open");
    constexpr size_t buf_size{ 1000lu };
    char buf[buf_size];
    ssize_t num_read;
    if ((num_read = read(config_fd, buf, buf_size - 1)) == -1)
        err_exit("read");
    buf[num_read] = '\0';
    log_message("Read config file: %s\n", buf);
}

static volatile sig_atomic_t hup_received{};

static void sighup_handler(int)
{
    hup_received = 1;
}

int main(int argc, char const *argv[])
{
    constexpr int sleep_time{ 5 };
    int count{};
    sigaction_st sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sighup_handler;
    if (sigaction(SIGHUP, &sa, nullptr) == -1)
        err_exit("sigaction");

    if (become_daemon(0) == -1)
        err_exit("become_daemon");

    log_open(log_file);
    read_config_file(config_file);

    int unslept{ sleep_time };
    while (true)
    {
        unslept = sleep(unslept);
        if (hup_received)
        {
            log_close();
            log_open(log_file);
            read_config_file(config_file);
            hup_received = 0;
        }

        if (unslept == 0)
        {
            ++count;
            log_message("Main: %d\n", count);
            unslept = sleep_time;
        }
    }

    return 0;
}

