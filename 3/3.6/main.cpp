// #undef _GNU_SOURCE
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <sys/ipc.h>
#include <sys/sem.h>

void demo1()
{
    char str[]{ "Hello world" };
    std::cout << strfry(str) << '\n';
}

void demo2()
{
    pid_t pid{ getpid() };
    printf("My pid is %jd\n", static_cast<std::intmax_t>(pid));
}

void demo3()
{
    sembuf s{ .sem_num{3}, .sem_op{-1}, .sem_flg{SEM_UNDO} };
}


int main(int argc, char const *argv[])
{
    demo2();
    return 0;
}

