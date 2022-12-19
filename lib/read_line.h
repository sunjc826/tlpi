#ifndef READ_LINE_H
#define READ_LINE_H
#include <cstddef>
#include <sys/types.h>
ssize_t read_line(int fd, void *buffer, size_t n);
#endif