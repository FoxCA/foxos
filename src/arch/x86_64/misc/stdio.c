

/* -- Includes ------------------------------------------------- */
#include <kbbuf.h>
#include <stdio.h>


/* -- Globals -------------------------------------------------- */
FILE *stdin = (FILE *)0x1234;


/* -- Functions ------------------------------------------------ */
int printf(char *s, ...) {
  char buffer[127];

  va_list valist;

  va_start(valist, _VABUFFERSIZE);

  for (int i = 0; s[i] != '\0'; i++) {
    if (s[i] == '\n') {
      putchar("\n");
    } else if (s[i] != '%') {
      putchar(s[i]);
    } else {
      switch (s[i+1]) {
        case '\0':
          break; //Protection
        case 'd': //Signed decimal integer
        case 'i':
          itos(va_arg(valist, int), buffer, 10);
          puts(buffer);
          i++;
          break;
        case 'o': //Octal integer
          itos(va_arg(valist, int), buffer, 8);
          puts(buffer);
          i++;
          break;
        case 'x': //Hexadecimal integer
          itos(va_arg(valist, int), buffer, 16);
          puts(buffer);
          i++;
          break;
        case '%': //%
          puts("%%");
        default: //Anything else
          return 1;
      }
    }
  }

  va_end(valist);

  return 0;
}


FILE *fopen(const char *path, const char *mode)
{
  return NULL;
}


int fgetc(FILE *stream)
{
  return 0;
}


char *fgets(char *s, int size, FILE *stream)
{
  return NULL;
}


int fputc(int c, FILE *stream)
{
  return 0;
}


int fclose(FILE *fp)
{
  return 0;
}


int fseek(FILE *stream, long offset, int whence)
{
  return 0;
}


long ftell(FILE *stream)
{
  return 0;
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return 0;
}


int putchar(int c)
{
  putch(c);
  return c;
}


int getchar(void)
{
  unsigned char ascii = 0;
  while(ascii == 0)
  {
    ascii = keyboard_dequeue();
  }
  return ascii;
}


char *itos(int i, char buffer[], int base) {
  const char digits[] = "0123456789abcdef";
  char *p = buffer;

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