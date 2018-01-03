#include <system.h>


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
  reboot();
}


void application_start(void)
{

  puts("");
  puts("JOS made by jonay2000 and a lot of help from the community\n");
  puts("Starting...\n");
  puts("READY.\n");
  OS();

  for(;;);
}

