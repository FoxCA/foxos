#ifndef STRUCTS_H
#define STRUCTS_H

#include <string.h>
#include <stdbool.h>

/* Common structs for OS/shell */

typedef struct
{
  int state;
  int loop;
  char input[79]; // TODO: replace with malloc resizing later on
} state;

typedef struct
{
  unsigned char flags;
  char key[11];
} key_press;

#endif
