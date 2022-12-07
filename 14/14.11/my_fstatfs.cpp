#include "tlpi_hdr.h"
#include <sys/statfs.h>
#include <sys/statvfs.h>
#include <alloca.h>

int main(int argc, char *argv[])
{
    char *file_path;

    if (argc == 1)
    {
        constexpr size_t buf_size{ 1000u };
        char *buf{ static_cast<char *>(alloca(buf_size)) };
        if (getcwd(buf, buf_size) == nullptr)
            err_exit("getcwd");
        file_path = buf;
    }
    else
        file_path = argv[1];

    typedef struct statfs statfs_st;
    typedef struct statvfs statvfs_st;

    //     struct statfs {
    //     __fsword_t f_type;    /* Type of filesystem (see below) */
    //     __fsword_t f_bsize;   /* Optimal transfer block size */
    //     fsblkcnt_t f_blocks;  /* Total data blocks in filesystem */
    //     fsblkcnt_t f_bfree;   /* Free blocks in filesystem */
    //     fsblkcnt_t f_bavail;  /* Free blocks available to
    //                              unprivileged user */
    //     fsfilcnt_t f_files;   /* Total inodes in filesystem */
    //     fsfilcnt_t f_ffree;   /* Free inodes in filesystem */
    //     fsid_t     f_fsid;    /* Filesystem ID */
    //     __fsword_t f_namelen; /* Maximum length of filenames */
    //     __fsword_t f_frsize;  /* Fragment size (since Linux 2.6) */
    //     __fsword_t f_flags;   /* Mount flags of filesystem
    //                              (since Linux 2.6.36) */
    //     __fsword_t f_spare[xxx];
    //     /* Padding bytes reserved for future use */
    // };

    // struct statvfs {
    //     unsigned long  f_bsize;    /* Filesystem block size */
    //     unsigned long  f_frsize;   /* Fragment size */
    //     fsblkcnt_t     f_blocks;   /* Size of fs in f_frsize units */
    //     fsblkcnt_t     f_bfree;    /* Number of free blocks */
    //     fsblkcnt_t     f_bavail;   /* Number of free blocks for
    //                                   unprivileged users */
    //     fsfilcnt_t     f_files;    /* Number of inodes */
    //     fsfilcnt_t     f_ffree;    /* Number of free inodes */
    //     fsfilcnt_t     f_favail;   /* Number of free inodes for
    //                                   unprivileged users */
    //     unsigned long  f_fsid;     /* Filesystem ID */
    //     unsigned long  f_flag;     /* Mount flags */
    //     unsigned long  f_namemax;  /* Maximum filename length */
    // };


    statfs_st file_path_fs_info;
    if (statfs(file_path, &file_path_fs_info) == -1)
        err_exit("statfs");

    std::printf("f_type: %lx\n", static_cast<unsigned long>(file_path_fs_info.f_type));

    statvfs_st file_path_vfs_info;
    if (statvfs(file_path, &file_path_vfs_info) == -1)
        err_exit("statvfs");

    std::printf("f_bsize: %lu\n", static_cast<unsigned long>(file_path_vfs_info.f_bsize));
    std::printf("f_frsize: %lu\n", static_cast<unsigned long>(file_path_vfs_info.f_frsize));
    std::printf("f_blocks: %lu\n", static_cast<unsigned long>(file_path_vfs_info.f_blocks));
    std::printf("f_bfree: %lu\n", static_cast<unsigned long>(file_path_vfs_info.f_bfree));
    std::printf("f_bavail: %lu\n", static_cast<unsigned long>(file_path_vfs_info.f_bavail));



    return 0;
}

