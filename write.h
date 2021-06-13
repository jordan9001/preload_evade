#ifndef WRITE_H
#define WRITE_H

#include <stddef.h>

#define litwrite(str) dowrite(str "\n", sizeof(str))

__attribute__((always_inline))
static inline void dowrite(const char* str, size_t len)
{
    __asm__(
    "mov $1, %%rax\n\t"
    "xor %%rdi, %%rdi\n\t"
    "syscall\n\t"
    :
    : "S" (str), "d" (len)
    : "rax", "rdi", "rcx", "r11", "cc"
    );
}

#endif
