

#ifndef __STRING_H__
#define __STRING_H__

#include <types.h>
#include <list.h>

/* -- Prototypes ----------------------------------------------- */
void nullString(char *s, int l);
char *itoa(int i, char buffer[], int base);
//in dtoa.c
char * dtoa(double f, char * buf, int precision);
int atoi(char * str);
int atoi_hex(char * str);

int isdigit(char c);
int isnumber(char * c);
int isspace(char c);


char *strcpy(char *dst, const char *src);
char *strcat(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
int strlen(char *str);
int strncmp( const char * s1, const char * s2, size_t n );

char * strdup(char * src);
char * strsep(char ** stringp, const char * delim);
list_t * str_split(char * str, const char * delim, unsigned int * numtokens);
char * list2str(list_t * list, const char * delim);
char * strstr(const char *in, const char *str);

#endif
