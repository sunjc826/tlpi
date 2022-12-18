#ifndef US_XFR_H
#define US_XFR_H
#include "tlpi_hdr.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

constexpr char *sv_sock_path{ "/tmp/us_xfr" };
constexpr int buf_size{ 100 };
#endif