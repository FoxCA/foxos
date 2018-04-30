#ifndef STDLIB_H
#define STDLIB_H

//itoa.c
char * itoa(int i, char * b, int base);

//atoi.c
int atoi(char * s);
int atoi_hex(char * s);

//dtoa.c
char * dtoa(double d, char * b, int precision);

//atod.c
//WIP

#endif