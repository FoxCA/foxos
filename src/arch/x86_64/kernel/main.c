
#include <stdio.h>
#include <system.h>
#include <unistd.h>
#include <paging.h>
#include <pmm.h>
#include <kheap.h>
#include <tss.h>

#include <vfs.h>
#include <ext2.h>
#include <ata.h>
#include <pci.h>
#include <pic.h>

#include <vga.h>
#include <vesa.h>

#include <string.h>
#include <rtc.h>
#include <spinlock.h>

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, int val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = (char)val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

void outportb(uint16_t port, uint8_t val) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(val));
}

/*
 * read a byte
 * */
uint8_t inportb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/*
 * Read 2 bytes
 * */
uint16_t inports(uint16_t _port) {
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/*
 * Write 2 bytes
 * */
void outports(uint16_t _port, uint16_t _data) {
    asm volatile ("outw %1, %0" : : "dN" (_port), "a" (_data));
}

/*
 * Read 4 bytes
 * */
uint32_t inportl(uint16_t _port) {
    uint32_t rv;
    asm volatile ("inl %%dx, %%eax" : "=a" (rv) : "dN" (_port));
    return rv;
}

/*
 * Write 4 bytes
 * */
void outportl(uint16_t _port, uint32_t _data) {
    asm volatile ("outl %%eax, %%dx" : : "dN" (_port), "a" (_data));
}

char getch(){
    char kbval;
    while((kbval=keyboard_dequeue())==0);
    return kbval;
}

void main()
{   
    #if GRAPHICS == 1 && SERIAL
        //qemu serial
        outportb(0x3f8 + 1, 0x00);
        outportb(0x3f8 + 3, 0x80);
        outportb(0x3f8 + 0, 0x03);
        outportb(0x3f8 + 1, 0x00);
        outportb(0x3f8 + 3, 0x03);
        outportb(0x3f8 + 2, 0xC7);
        outportb(0x3f8 + 4, 0x0B);
    #endif

    init_video();
    autoscroll();

    settextcolor(green,black);
    printf("vga initialized...\n");

    settextcolor(yellow,black);
    gdt_init();
    settextcolor(green,black);
    printf("global descriptor table initialized...\n");
    settextcolor(yellow,black);
    idt_init();   
    settextcolor(green,black);
    printf("interrupt desctiptor table initialized...\n");
    settextcolor(yellow,black);
    isrs_install();
    settextcolor(green,black);
    printf("interrupt service routines initialized...\n");

    #if 0
    settextcolor(yellow,black);
    tss_init(5, 0x10, 0);
    settextcolor(green,black);
    printf("task state segment initialized...\n");
    #endif

    settextcolor(yellow,black);
    keyboard_install();
    settextcolor(green,black);
    printf("keyboard initialized...\n");

    settextcolor(yellow,black);


    pmm_init(1096 * M);
    settextcolor(green,black);
    printf("physical memory manager initialized...\n");
    settextcolor(yellow,black);
    paging_init();
    settextcolor(green,black);
    printf("paging initialized...\n");

    settextcolor(yellow,black);
    kheap_init(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX_ADDRESS); 
    settextcolor(green,black);
    printf("heap set up...\n");

    settextcolor(yellow,black);
    timer_init();
    settextcolor(green,black);
    printf("timer initialized...\n");

    #if FILESYSTEM
    settextcolor(yellow,black);
    pci_init();
    settextcolor(green,black);
    printf("pci initialized...\n");
    settextcolor(yellow,black);
    vfs_init();
    settextcolor(green,black);
    printf("virtual filesystem initialized...\n");
    settextcolor(yellow,black);
    ata_init();
    settextcolor(green,black);
    printf("ata initialized...\n");
    settextcolor(yellow,black);
    ext2_init("/dev/hda", "/");
    settextcolor(green,black);
    printf("filesystem initialized...\n");
    #endif
    settextcolor(yellow,black);
    rtc_init();
    settextcolor(green,black);
    printf("real-time clock initialized\n");
    

    settextcolor(yellow,black);
    uint32_t esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    settextcolor(green,black);


    printf("Fox Kernel ");
    printf(FOX_VERSION);
    printf(" loaded.\n");
    printf("Current date and time: %s (may be wrong, didnt make timezones yet...)\n", get_current_datetime_str());
    printf("boot sequence complete\n");
    printf("press any key to continue\n");

    autoscroll();

    settextcolor(white,black);
    getch() ; 
    cls();

    settextcolor(white,black);
    
    #if GRAPHICS == 0
        application_start();
    #endif
    #if GRAPHICS == 1
        vesa_init();
        gui_init();
        application_start();
    #endif


    for (;;);
}
