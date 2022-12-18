#ifndef UD_UCASE_H
#define UD_UCASE_H

#include "tlpi_hdr.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <cctype>

constexpr int buf_size{ 10 };
constexpr const char *sv_sock_path{ "/tmp/ud_ucase" };

#endif