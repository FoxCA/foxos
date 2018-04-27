

/* -- Includes ------------------------------------------------- */
#include <kbbuf.h>
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <system.h>
#include <version.h>

/* -- Globals -------------------------------------------------- */
FILE *stdin = (FILE *)0x1234;


/* -- Functions ------------------------------------------------ */
int printf(char *s, ...) {
  char buff[128];
  char *buffer = buff;

  va_list valist;

  va_start(valist, _VABUFFERSIZE);

  for (; *s != '\0'; s++) {
    if (*s == '\n') {
      putchar('\n');
      // putchar('\n');
    } else if (*s != '%') {
      putchar(*s);
    } else {
      s++;
      switch (*s) {
        case '\0':
          break; //Protection
        case 'd': //Signed decimal integer
        case 'i':
          itoa(va_arg(valist, int), buffer, 10);
          puts(buffer);
          s++;
          break;
        case 's':
          puts(va_arg(valist, char *));
          s++;
          break;
        case 'o': //Octal integer
          itoa(va_arg(valist, int), buffer, 8);
          puts(buffer);
          s++;
          break;
        case 'x': //Hexadecimal integer
          itoa(va_arg(valist, int), buffer, 16);
          puts(buffer);
          s++;
          break;
        case 'p':
          itoa((uint32_t)va_arg(valist, void *), buffer, 16);
          puts(buffer);
          s++;         
          break;
        case 'f':
          dtoa(va_arg(valist, double), buffer, -1);
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

int getchar(void)
{
  unsigned char ascii = 0;
  while(ascii == 0)
  {
    ascii = keyboard_dequeue();
  }
  return ascii;
}


void panic(char* message, char* file, uint32_t line)
{
  asm volatile ("cli");
  settextcolor(red,black);

  printf("PANIC(");
  printf(message);
  printf(") at ");
  printf(file);
  printf(":");
  printf("%i",line);
  printf("\n\n");


  printf("please reboot");  
  
  for (;;);
}

void panic_assert(char* file, uint32_t line, char* desc)
{
  asm volatile ("cli");
  settextcolor(red,black);

  printf("ASSERTION-FAILED(");
  printf(desc);
  printf(") at ");
  printf(file);
  printf(":");
  printf("%i",line);
  printf("\n\n");

  printf("please reboot");  

  for (;;);
}