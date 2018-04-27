#include <system.h>
#include <stdio.h>

void reboot(void)
{
  unsigned char good = 0x02;
  while (good & 0x02)
  {
    good = inportb(0x64);
  }
  outportb(0x64, 0xFE);
  loop:
    asm volatile ("hlt");
  goto loop;
}


void shutdown(void)
{
  outportb(0xf4, 0x00);
}


void application_start(void)
{
  
  os(); // "loaded" output handled by shell

  for(;;);
}
