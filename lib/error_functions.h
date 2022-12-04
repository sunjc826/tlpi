// Error diagnostic functions
#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#ifdef __GNUC__
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif


void err_msg(const char *format, ...);

// exit with error number given by errno
void err_exit(const char *format, ...);

// exit with error number given by errnum
void err_exit(int errnum, const char *format, ...);

// calls _exit instead of exit
void err_exit_(const char *format, ...);

// used to diagnose general errors
void fatal(const char *format, ...);

void usage_err(const char *format, ...);

void cmd_line_err(const char *format, ...);


#endif
