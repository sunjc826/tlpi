#include <cstdarg>
#include "error_functions.h"
#include "tlpi_hdr.h"
#include "ename.h"

#ifdef __GNUC__
__attribute__((__noreturn__))
#endif
static void terminate(bool use_exit3)
{
    char *s;
    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort(); // dump core
    else if (use_exit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

static void output_error(bool flush_stdout, const char *format, va_list ap)
{
    constexpr int buf_size{ 500 };
    char buf[buf_size * 3], user_msg[buf_size], err_text[buf_size];

    vsnprintf(user_msg, buf_size, format, ap);

    snprintf(err_text, buf_size, ":");

    snprintf(buf, buf_size * 3, "ERROR%s %s\n", err_text, user_msg);

    if (flush_stdout)
        fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);
}

static void output_error(int err, bool flush_stdout, const char *format, va_list ap)
{
    constexpr int buf_size{ 500 };
    char buf[buf_size * 3], user_msg[buf_size], err_text[buf_size];

    vsnprintf(user_msg, buf_size, format, ap);

    snprintf(err_text, buf_size, " [%s %s]",
        (err > 0 && err <= MAX_ENAME) ?
        ename[err] : "?UNKNOWN?", strerror(err));

    snprintf(buf, buf_size * 3, "ERROR%s %s\n", err_text, user_msg);

    if (flush_stdout)
        fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);
}

void err_msg(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int saved_errno{ errno };
    output_error(true, errno, format, ap);
    va_end(ap);

    errno = saved_errno;
}

static void err_exitv(const char *format, va_list ap, int err_num = errno)
{
    output_error(err_num, true, format, ap);
    terminate(true);
}

void err_exit(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    err_exitv(format, ap);
    va_end(ap);
}

void err_exit_(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    output_error(errno, false, format, ap);
    va_end(ap);

    terminate(false);
}

void err_exit(int err_num, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    err_exitv(format, ap, err_num);
    va_end(ap);
}

void fatal(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    output_error(true, format, ap);
    va_end(ap);

    terminate(true);
}

void usage_err(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);

    fflush(stdout);

    fputs("Usage: ", stderr);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

void cmd_line_err(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    fflush(stdout);

    fputs("Command-line usage error: ", stderr);
    vfprintf(stderr, format, ap);

    va_end(ap);

    fflush(stderr);
    exit(EXIT_FAILURE);
}
