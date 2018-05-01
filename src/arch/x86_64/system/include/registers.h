
#ifndef REGISTERS_H
#define REGISTERS_H

typedef struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
}regs;

void print_regs(regs * r);

#endif
