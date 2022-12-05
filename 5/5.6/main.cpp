#include "tlpi_hdr.h"

// pread is equivalent to atomically executing the following
void pread_equiv(int fd, void *buf, size_t buf_size, off_t offset)
{
    off_t current_offset{ lseek(fd, 0, SEEK_CUR) };
    lseek(fd, offset, SEEK_SET);
    read(fd, buf, buf_size);
    lseek(fd, current_offset, SEEK_SET);
}
