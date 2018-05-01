
#include <kernel.h>

void putc(char c){
	shell_putc(c);
}


void putc_qemu(char c){
	#if SERIAL
    while ((inportb(0x3f8 + 5) & 0x20) == 0);
    outportb(0x3f8,c);		
	#endif
}

void puts(char * s){
	for (; *s != '\0'; s++)
	{
		putc(*s);
	}
}

void puts_qemu(char * s){
	for (; *s != '\0'; s++)
	{
		putc_qemu(*s);
	}
}
