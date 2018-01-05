

/* -- Includes ------------------------------------------------- */
#include <string.h>
#include <stdio.h>

/* -- Internal Constants --------------------------------------- */
#define ITOS_MAX_BUFFER 128

/* -- Functions ------------------------------------------------ */

char *itos(int i, char *buffer, int base) {
  const char digits[17] = "0123456789abcdef";
  char pt[ITOS_MAX_BUFFER];
  char *p = &pt;

  if (i < 0) {
    *p++ = '-';
    i *= -1;
  }

  int shifter = i;

  do { //Find out the number of digits
    ++p;
    shifter /= base;
  } while (shifter);
  *p = '\0';
  do { //Insert the digits into the new string one by one
    *--p = digits[i%base];
    i = i / base;
  } while (i);

  return buffer;
}

char *strcpy(char *dst, const char *src)
{
  unsigned int i = 0;
  for(i=0; src[i] != 0; i++)
  {
    dst[i] = src[i];
  }
  dst[i] = 0;
  return dst;
}


char *strcat(char *dst, const char *src)
{
  unsigned int i = 0;
  unsigned int j = 0;
  for (i = 0; dst[i] != 0; i++)
  {
  }
  for (j = 0; src[j] != 0; j++)
  {
    dst[i+j] = src[j];
  }
  dst[i+j] = 0;
  return dst;
}


int strcmp(const char *s1, const char *s2)
{
 while(*s1 && (*s1 == *s2))
 {
   s1++;
   s2++;
 }
 return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
