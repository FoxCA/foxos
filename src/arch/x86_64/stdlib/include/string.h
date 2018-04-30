
#ifndef STRING_H
#define STRING_H

#include <list.h>


void nullString(char *s, int l);

//stringtype.c
int isdigit(char c);
int isnumber(char * c);
int isspace(char c);


//string.c
int starts_with(char *s1, char* s2);
char *strcpy(char *dst, char *src);
char *strcat(char *dst, char *src);
int strcmp(char *s1, char *s2);
int strlen(char *str);
int strncmp(char * s1, char * s2, size_t n );

char * strdup(char * src);
char * strsep(char ** stringp, char * delim);
list_t * str_split(char * str, char * delim, unsigned int * numtokens);
char * list2str(list_t * list, char * delim);
char * strstr(char *in, char *str);


//string_memory.c
void * memcpy(void *dest, void *src, size_t count);
void * memset(void *dest, int val, size_t count);
unsigned short * memsetw(unsigned short *dest, unsigned short val, size_t count);


#endif