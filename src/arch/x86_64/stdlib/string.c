/* -- Includes ------------------------------------------------- */
#include <kernel.h>

/*
nulls a sring given a pointer to the string and a length
*/
void nullString(char *s, int l)
{
    int i;
	for (i = 0; i != l; i++)
	{
		*s = 0x00;
		s++;
	}
}


/*
copies the contents of 1 string to another string.
*/
char *strcpy(char *dst, char *src)
{
  unsigned int i;
  for(i=0; src[i] != 0; i++)
  {
    dst[i] = src[i];
  }
  dst[i] = 0;
  return dst;
}

/*
adds the contents of src string to the end of dst
*/
char *strcat(char *dst, char *src)
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

/*
compares 2 strings. returns 0 if they are the same
*/
int strcmp(char *s1, char *s2)
{
  int i;
  for (i = 0; ; i++)
  {
      if (s1[i] != s2[i])
      {
          return s1[i] < s2[i] ? -1 : 1;
      }

      if (s1[i] == '\0')
      {
          return 0;
      }
  }
}

/*
like strcmp, compares the first n characters
*/
int strncmp( char * s1, char * s2, size_t n )
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


/*
returns the length of a string
*/
int strlen(char *str)
{
	int i;
	for (i = 0;; i++)
	{
		if (str[i] == '\0') return i;
	}
	return 0; // Theoretically impossible, but better safe than sorry.
}


/*
returns 1 if s1 starts with the same text as s2
*/
int starts_with(char *s1, char* s2)
{
  if(strncmp(s1, s2, strlen(s2)) == 0){
    return 1;
  }
  return 0;
}


/*
makes a list of strings from a larger string, splits up by delim.
*/
list_t * str_split(char * str, char * delim, unsigned int * numtokens) {
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


/*
opposite to str_split.
*/
char * list2str(list_t * list, char * delim) {
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

/*
finds first occurence of str in the input; returns pointer to that point.
*/
char * strstr(char *in, char *str) {
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


/*
extracts a token from a string. see http://man7.org/linux/man-pages/man3/strsep.3.html
*/
char *strsep(char **stringp, char *delim) {
    char *s;
    char *spanp;
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


/*
duplicates a string
*/
char * strdup(char * src) {
    int len = strlen(src) + 1;
    char * dst = kmalloc(len);
    memcpy(dst, src, len);
    return dst;
}