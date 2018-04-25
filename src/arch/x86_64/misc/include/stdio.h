
#ifndef __STDIO_H__
#define __STDIO_H__


/* -- Includes ------------------------------------------------- */
#include <system.h>
#include <stdarg.h>
#include <stdlib.h>

/* -- Defines -------------------------------------------------- */
#define EOF 0
#define true 1
#define false 0
#define NULL (void *)0
#define SEEK_END 0
#define SEEK_SET 0


/* -- Types ---------------------------------------------------- */
typedef void FILE;
#include <types.h>

/* -- Globals -------------------------------------------------- */
extern FILE *stdin;


/* -- Prototypes ----------------------------------------------- */
int printf(char *s, ...);

FILE *fopen(const char *path, const char *mode);
int fgetc(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
int fputc(int c, FILE *stream);
int fclose(FILE *fp);

int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

int putchar(int c);
int getchar(void);

void panic(const char* message, const char* file, uint32_t line);
void panic_assert(const char* file, uint32_t line, const char* desc);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void) 0 : panic_assert(__FILE__, __LINE__, #b));

#endif
