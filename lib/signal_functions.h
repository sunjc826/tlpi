#ifndef SIGNAL_FUNCTIONS_H
#define SIGNAL_FUNCTIONS_H
#include <signal.h>
#include <cstdio>
void print_sigset(FILE *of, const char *prefix, const sigset_t *sigset);

int print_sig_mask(FILE *of, const char *msg);

int print_pending_sigs(FILE *of, const char *msg);
#endif
