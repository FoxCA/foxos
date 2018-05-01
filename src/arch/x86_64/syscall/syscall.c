#include <kernel.h>


void * syscall_table[NUM_SYSCALLS] = {
    vfs_create_file,
    schedule,
    printf,
    create_process_from_routine,
    _exit
};

void syscall_dispatcher(regs * regs) {
    if(regs->eax >= NUM_SYSCALLS) return;
    void * system_api = syscall_table[regs->eax];
    int ret;
    memcpy(&saved_context, regs, sizeof(regs));
    asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (system_api));

    // I don't beleive this would set eax to return value ?
    regs->eax = ret;
}

void syscall_init() {
    irq_install_handler(0x80, syscall_dispatcher);
}