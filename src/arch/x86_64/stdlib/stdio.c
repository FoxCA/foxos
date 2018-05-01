#include <kernel.h>

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