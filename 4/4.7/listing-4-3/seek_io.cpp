#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <cctype>

int main(int argc, char const *argv[])
{
    if (argc < 3 || std::strcmp(argv[1], "--help") == 0)
        usage_err("%s file {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);

    int fd{ open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) };
    if (fd == -1)
        err_exit("open");

    for (int ap{ 2 }; ap < argc; ++ap)
    {
        switch (argv[ap][0])
        {
        case 'r':
        case 'R':
        {
            long len{ get_long(&argv[ap][1], GN_ANY_BASE, argv[ap]) };
            char *buf{ new char[len] };
            if (buf == nullptr)
                err_exit("new");

            long num_read{ read(fd, buf, static_cast<size_t>(len)) };
            if (num_read == -1)
                err_exit("read");

            if (num_read == 0)
                std::printf("%s: end-of-file\n", argv[ap]);
            else
            {
                std::printf("%s: ", argv[ap]);
                for (int i{ 0 }; i < num_read; i++)
                {
                    if (argv[ap][0] == 'r')
                        std::printf("%c", std::isprint(static_cast<unsigned char>(buf[i])) ? buf[i] : '?');
                    else
                        std::printf("%02x ", static_cast<unsigned int>(buf[i]));
                }
                std::printf("\n");
            }

            delete[] buf;
            break;
        }
        case 'w':
        {
            long num_written{ static_cast<long>(write(fd, &argv[ap][1], std::strlen(&argv[ap][1]))) };
            if (num_written == -1)
                err_exit("write");
            std::printf("%s: wrote %ld bytes\n", argv[ap], num_written);
            break;
        }
        case 's':
        {
            off_t offset{ static_cast<off_t>(get_long(&argv[ap][1], GN_ANY_BASE, argv[ap])) };
            if (lseek(fd, offset, SEEK_SET) == -1)
                err_exit("lseek");
            std::printf("%s: seek succeeded\n", argv[ap]);
            break;
        }
        default:
            cmd_line_err("Argument must start with [rRws]: %s\n", argv[ap]);
        }
    }

    return 0;
}

