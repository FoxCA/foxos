#include <kernel.h>

#define ITOA_MAX_BUFFER 128

/*
converts an integer to a string
*/
char *itoa(int i, char *buffer, int base) {
  const char digits[17] = "0123456789abcdef";
  char pt[ITOA_MAX_BUFFER];
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
