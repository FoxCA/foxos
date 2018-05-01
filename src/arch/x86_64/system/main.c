#include <kernel.h>

void main()
{   
    #if SERIAL
        //qemu serial
        outportb(0x3f8 + 1, 0x00);
        outportb(0x3f8 + 3, 0x80);
        outportb(0x3f8 + 0, 0x03);
        outportb(0x3f8 + 1, 0x00);
        outportb(0x3f8 + 3, 0x03);
        outportb(0x3f8 + 2, 0xC7);
        outportb(0x3f8 + 4, 0x0B);
    #endif

    vga_init_video();
    linescroll();

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

    //not loading this, it crashes :P
    #if 1
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


    settextcolor(yellow,black);
    bios32_init();
    settextcolor(green,black);
    printf("bios32 driver initialized...\n");

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
    fs_init();
    settextcolor(green,black);
    printf("filesystem initialized...\n");
    #endif
    settextcolor(yellow,black);
    rtc_init();
    settextcolor(green,black);
    printf("real-time clock initialized\n");
    
    settextcolor(yellow,black);
    process_init();
    settextcolor(green,black);
    printf("processes initialized\n");

    settextcolor(yellow,black);
    syscall_init();
    settextcolor(green,black);
    printf("syscalls initialized\n");

    settextcolor(yellow,black);
    uint32_t esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    tss_set_stack(0x10, esp);
    settextcolor(green,black);

    printf("Fox Kernel ");
    printf(FOX_VERSION);
    printf(" loaded.\n");
    printf("Current date and time: %s (may be wrong, didnt make timezones yet...)\n", get_current_datetime_str());
    printf("boot sequence complete\n");
    printf("press any key to continue\n");

    autoscroll();

    settextcolor(white,black);
    getc(); 
    cls();

    settextcolor(white,black);
    
    #if GRAPHICS == 0
        create_process_from_routine(kcll_start,"user process");
    #endif
    #if GRAPHICS == 1
        vesa_init();
        gui_init();
        application_start();
    #endif


    for (;;);
}
