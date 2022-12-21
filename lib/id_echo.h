#ifndef ID_ECHO_H
#define ID_ECHO_H
#include "inet_sockets.h"
#include "tlpi_hdr.h"

constexpr const char *service{ "echo" };
constexpr size_t buf_size{ 500lu };
#endif