#ifndef INET_SOCKETS_H
#define INET_SOCKETS_H

#include <sys/socket.h>
#include <netdb.h>

int inet_connect(const char *host, const char *service, int type);

int inet_listen(const char *service, int backlog, socklen_t *addr_len);

int inet_bind(const char *service, int type, socklen_t *addr_len);

char *inet_address_str(const sockaddr *addr, socklen_t addr_len, char *addr_str, int addr_str_len);

constexpr size_t inet_sock_addr_str_len{ 4096lu };

#endif