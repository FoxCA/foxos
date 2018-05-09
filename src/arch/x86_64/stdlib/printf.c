
#include <kernel.h>



/*
	default printf function like in ansi-c
	takes format (char * s) and arguments to fill the format.
*/
int printf(char *s, ...) {
  char buff[128];
  char *buffer = buff;

  va_list valist;

  va_start(valist, s);

  for (; *s != '\0'; s++) {
    if (*s == '\n') {
      putc('\n');
      // putchar('\n');
    } else if (*s != '%') {
      putc(*s);
    } else {
      s++;
      switch (*s) {
        case '\0':
          break; //Protection
        case 'd': //Signed decimal integer
        case 'i':
          itoa(va_arg(valist, int), buffer, 10);
          puts(buffer);
          break;
        case 's':
          puts(va_arg(valist, char *));
          break;
        case 'o': //Octal integer
          itoa(va_arg(valist, int), buffer, 8);
          puts(buffer);
          break;
        case 'x': //Hexadecimal integer
          itoa(va_arg(valist, int), buffer, 16);
          puts(buffer);
          break;
        case 'p':
          itoa((uint32_t)va_arg(valist, void *), buffer, 16);
          puts(buffer);
          break;
        case 'f':
          dtoa(va_arg(valist, double), buffer, -1);
          puts(buffer);
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

/*
	like printf except this function prints to the qemu-console (on com0). doesn't really work on a 
	real computer of course ¯\_(ツ)_/¯
*/
int printf_qemu(char *s, ...) {
  char buff[128];
  char *buffer = buff;

  va_list valist;

  va_start(valist, s);

  for (; *s != '\0'; s++) {
    if (*s == '\n') {
      putc_qemu('\n');
    } else if (*s != '%') {
      putc_qemu(*s);
    } else {
      s++;
      switch (*s) {
        case '\0':
          break; //Protection
        case 'd': //Signed decimal integer
        case 'i':
          itoa(va_arg(valist, int), buffer, 10);
          puts_qemu(buffer);
          break;
        case 's':
          puts_qemu(va_arg(valist, char *));
          break;
        case 'o': //Octal integer
          itoa(va_arg(valist, int), buffer, 8);
          puts_qemu(buffer);
          break;
        case 'x': //Hexadecimal integer
          itoa(va_arg(valist, int), buffer, 16);
          puts_qemu(buffer);
          break;
        case 'p':
          itoa((uint32_t)va_arg(valist, void *), buffer, 16);
          puts_qemu(buffer);
          break;
        case 'f':
          dtoa(va_arg(valist, double), buffer, -1);
          puts_qemu(buffer);
          break;
        case '%': //%
          puts_qemu("%%");
        default: //Anything else
          return 1;
      }
    }
  }

  va_end(valist);

  return 0;
}