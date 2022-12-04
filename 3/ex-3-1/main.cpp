#include <unistd.h>
#include <linux/reboot.h>
#include <cstdio>
int main(int argc, char const *argv[])
{
    constexpr unsigned int
        magic1{ LINUX_REBOOT_MAGIC1 },
        magic2{ LINUX_REBOOT_MAGIC2 }, // Linus Torvald's birthday
        // Birthday of Linus' 3 daughters
        magic2a{ LINUX_REBOOT_MAGIC2A },
        magic2b{ LINUX_REBOOT_MAGIC2B },
        magic2c{ LINUX_REBOOT_MAGIC2C };

    std::printf("%x\n%x\n%x\n%x\n%x\n", magic1, magic2, magic2a, magic2b, magic2c);

    return 0;
}
