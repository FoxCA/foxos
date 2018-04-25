

#ifndef __STRING_H__
#define __STRING_H__

#include <types.h>

/* -- Prototypes ----------------------------------------------- */
void nullString(char *s, int l);
char *itos(int i, char buffer[], int base);
char *strcpy(char *dst, const char *src);
char *strcat(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
int strlen(char *str);
int strncmp( const char * s1, const char * s2, size_t n );

#endif
