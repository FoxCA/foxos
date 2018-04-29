OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)
SECTIONS
{
    /*Kernel binary starts at 3GB. This is VMA, virtual memory address, so now all the variable, code label is going to be reference based on this VMA
     * 
     * */
    . = 0xC0100000;
    /*
        *(.section name) means: all sections named "section name" in all input object files, please put them here
        multiboot is a section defined in entry.asm, it contains information for grub to find where our kernel entry point is

       use keyword AT to relocate text section, same for rodata, data and bss section
    */
    .text ALIGN(4K) : AT(ADDR(.text)-0xC0000000)
    {
        *(.multiboot)
        *(.text)
    }
    .rodata ALIGN (4K) : AT(ADDR(.rodata)-0xC0000000)
    {
        *(.rodata)
    }
     /*
        .bss is also data section, but it contains uninitialized data
        .common is also uninitialized data section
        .initial_stack is a section defined in entry.asm, as an initial stack for our kernel
    */
    .bss ALIGN(4K) : AT(ADDR(.bss)-0xC0000000)
    {
        *(.COMMON)
        *(.bss)
        *(.initial_stack)
    }
    .data ALIGN(4K) : AT(ADDR(.data)-0xC0000000)
    {
        *(.data)
    }

    /*
        Put a symbol end here, it tells us where all the kernel code/data ends, it means everything after 'end' can be used for something else
    */
    end = .;
}