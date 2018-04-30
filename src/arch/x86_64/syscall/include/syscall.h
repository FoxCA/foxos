#ifndef SYSCALL_H
#define SYSCALL_H

#include <registers.h>

#define NUM_SYSCALLS 5

extern void * syscall_table[NUM_SYSCALLS];

void syscall_dispatcher(regs * regs);

void syscall_init();

void _exit();


#endif