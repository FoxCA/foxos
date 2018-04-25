

/* -- Includes ------------------------------------------------- */
#include <string.h>
#include <stdio.h>
#include <types.h>

/* -- Internal Constants --------------------------------------- */
#define ITOS_MAX_BUFFER 128

/* -- Functions ------------------------------------------------ */

void nullString(char *s, int l) // Nulls a string given a &pointer and a length.
{
	for (int i = 0; i != l; i++)
	{
		*s = 0x00;
		s++;
	}
}

char *itos(int i, char *buffer, int base) {
  const char digits[17] = "0123456789abcdef";
  char pt[ITOS_MAX_BUFFER];
  char *p = pt;

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

  return strcpy(buffer, p);
}

char *strcpy(char *dst, const char *src)
{
  unsigned int i;
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
 while ((*s1 == '\0' || *s2 == '\0') && (*s1 == *s2))
 {
   s1++;
   s2++;
 }
 return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp( const char * s1, const char * s2, size_t n )
{
    while ( n && *s1 && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
        --n;
    }
    if ( n == 0 )
    {
        return 0;
    }
    else
    {
        return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
    }
}

int strlen(char *str)
{
	for (int i = 0;; i++)
	{
		if (str[i] == '\0') return i;
	}
	return 0; // Theoretically impossible, but better safe than sorry.
}

int strswith(char *str, char *sw) {
  if (strncmp(str,sw,strlen(sw)) == 0) {
    return 1;
  };
  return 0;
}
