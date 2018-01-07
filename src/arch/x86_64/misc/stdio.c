

/* -- Includes ------------------------------------------------- */
#include <kbbuf.h>
#include <stdio.h>


/* -- Globals -------------------------------------------------- */
FILE *stdin = (FILE *)0x1234;


/* -- Functions ------------------------------------------------ */
int printf(char *s, ...) {
  char buff[128];
  char *buffer = &buff;

  va_list valist;

  va_start(valist, _VABUFFERSIZE);

  for (; *s != '\0'; s++) {
    if (*s == '\n') {
      putchar('\n');
    } else if (*s != '%') {
      putchar(*s);
    } else {
      switch (*s) {
        case '\0':
          break; //Protection
        case 'd': //Signed decimal integer
        case 'i':
          itos(va_arg(valist, int), buffer, 10);
          puts(buffer);
          s++;
          break;
        case 'o': //Octal integer
          itos(va_arg(valist, int), buffer, 8);
          puts(buffer);
          s++;
          break;
        case 'x': //Hexadecimal integer
          itos(va_arg(valist, int), buffer, 16);
          puts(buffer);
          s++;
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

void getline(char *buffer, int buf_length)
{
  int in_char;
  int i = 0;
  while ((char)in_char != '\n' && i < buf_length)
  {
    in_char = getchar();
    *buffer++ = (char)in_char;
    i++;
    putchar(in_char);
  }
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
