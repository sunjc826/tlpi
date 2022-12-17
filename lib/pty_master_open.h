#ifndef PTY_MASTER_OPEN
#define PTY_MASTER_OPEN
#include <cstddef>

int pty_master_open(char *slave_name, size_t sn_len);
#endif