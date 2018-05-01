#include <kernel.h>


int getc(void)
{
  unsigned char ascii = 0;
  while(ascii == 0)
  {
    ascii = keyboard_dequeue();
  }
  return ascii;
}