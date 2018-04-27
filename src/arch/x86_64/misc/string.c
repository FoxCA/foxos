

/* -- Includes ------------------------------------------------- */
#include <string.h>
#include <list.h>
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

char *itoa(int i, char *buffer, int base) {
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


int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

list_t * str_split(char * str, const char * delim, unsigned int * numtokens) {
    list_t * ret_list = list_create();
    char *s = strdup(str);
    char *token, *rest = s;
    while ((token = strsep(&rest, delim)) != NULL) {
        if(!strcmp(token, ".")) continue;
        if(!strcmp(token, "..")) {
            if(list_size(ret_list) > 0) list_pop(ret_list);
            continue;
        }
        list_push(ret_list, strdup(token));
        if(numtokens) (*numtokens)++;
    }
    free(s);
    return ret_list;
}

char * list2str(list_t * list, const char * delim) {
    char * ret = kmalloc(256);
    memset(ret, 0, 256);
    int len = 0, ret_len = 256;
    while(list_size(list)> 0) {
        char * temp = list_pop(list)->val;
        int len_temp = strlen(temp);
        if(len + len_temp + 1 + 1 > ret_len) {
            ret_len = ret_len * 2;
            ret = krealloc(ret, ret_len);
            len = len + len_temp + 1;
        }
        strcat(ret, delim);
        strcat(ret, temp);
    }
    return ret;
}

char * strstr(const char *in, const char *str) {
    char c;
    uint32_t len;

    c = *str++;
    if (!c)
        return (char *) in;

    len = strlen(str);
    do {
        char sc;

        do {
            sc = *in++;
            if (!sc)
                return (char *) 0;
        } while (sc != c);
    } while (strncmp(in, str, len) != 0);

    return (char *) (in - 1);
}

char *strsep(char **stringp, const char *delim) {
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;
    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}

char * strdup(char * src) {
    int len = strlen(src) + 1;
    char * dst = kmalloc(len);
    memcpy(dst, src, len);
    return dst;
}