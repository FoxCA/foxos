#ifndef __SYSTEM_H
#define __SYSTEM_H

/* This defines what the stack looks like after an ISR was running */
typedef struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
}regs;


#include <types.h>
#include <time.h>
#include <gdt.h>
#include <idt.h>

#define LOAD_MEMORY_ADDRESS 0xC0000000
#define K 1024
#define M (1024*K)
#define G (1024*M)

/* MAIN.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, int val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern int strlen(char *str);
extern uint8_t inportb(uint16_t port);
extern void outportb(uint16_t port, uint8_t val);
extern uint16_t inports(uint16_t _port);
extern void outports(uint16_t _port, uint16_t _data);
extern uint32_t inportl(uint16_t _port);
extern void outportl(uint16_t _port, uint32_t _data);
extern char getch();

/* SCRN.C */
extern void set_csr_x(int x);
extern void set_csr_y(int y);
extern void set_csr_xy(int x, int y);
extern void init_video(void);
extern int puts(char *text);
extern int puts_qemu(char *text);
extern void putch(char c);
extern void putch_qemu(char c);
extern void cls();
extern void move_csr(void);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void settextcolorf(unsigned int);
extern unsigned int gettextcolorf();
extern void waitscroll();
extern void autoscroll();
extern void linescroll();
extern void slowscroll();


/* ISRS.C */
extern void isrs_install();

/* IRQ.C */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();

/* KEYBOARD.C */
extern int keyboard_initialized();
extern void keyboard_install();

/* START.ASM */
extern void sys_heap();

/* OS.C */
extern void os();

#endif
