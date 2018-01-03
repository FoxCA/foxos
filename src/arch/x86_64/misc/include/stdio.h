
#ifndef __STDIO_H__
#define __STDIO_H__


/* -- Includes ------------------------------------------------- */
#include <system.h>
#include <stdarg.h>

/* -- Defines -------------------------------------------------- */
#define EOF 0
#define true 1
#define false 0
#define NULL (void *)0
#define SEEK_END 0
#define SEEK_SET 0


/* -- Types ---------------------------------------------------- */
typedef void FILE;
typedef unsigned int size_t;


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

char *itos(int i, char buffer[], int base);

#endif

