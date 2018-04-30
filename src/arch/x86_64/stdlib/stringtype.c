#include <kernel.h>


int isdigit(char c) {
    return c >= '0' && c <= '9';
}

int isnumber(char * c) {
  int i;
  for (i = 0; c[i] != '\0'; ++i)
  {
    if(!(isdigit(c[i]) || isspace(c[i]))){
      return 0;
    }
  }
  return 1;
}

int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}