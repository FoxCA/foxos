

#ifndef KERNEL_H
#define KERNEL_H

#include <options.h>

//error
#include <errno.h>

//stdlib
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

//datastructures
#include <string.h>
#include <list.h>

//system
#include <pc.h>
#include <boot.h>

//time
#include <time.h>
#include <rtc.h>

//interrupts
#include <isrs.h>
#include <idt.h>
#include <irq.h>
#include <gdt.h>

//memory
#include <memory.h>
#include <registers.h>
#include <pmm.h>
#include <kheap.h>
#include <paging.h>

//video
//general
#include <video.h>

//vga
#include <colors.h>
#include <vga.h>

//vesa


//keyboard
#include <keyboard.h>
#include <kbbuf.h>

//multiprocessing
#include <tss.h>
#include <process.h>
#include <pic.h>
#include <usermode.h>
#include <thread.h>

//filesystem
#include <ata.h>
#include <ext2.h>
#include <pci.h>
#include <vfs.h>
#include <fs.h>

//executable
#include <elf.h>


//kcll
#include <kcll_keyboard.h>
#include <kcll.h>

//bios
#include <bios32.h>

//syscall
#include <syscall.h>

#endif