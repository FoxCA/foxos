
#ifndef STDIO_H
#define STDIO_H

#define NULL 0

#include <stdint.h>

//printf.c
int printf(char *, ...);
int printf_qemu(char *, ...);

//get.c
int getc();

//put.c
void putc(char);
void putc_qemu(char);

void puts(char *);
void puts_qemu(char *);

//stdio.c
void panic(char* message, char* file, uint32_t line);
void panic_assert(char* file, uint32_t line, char* desc);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void) 0 : panic_assert(__FILE__, __LINE__, #b));


#endif