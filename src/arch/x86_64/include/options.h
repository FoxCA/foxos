
#ifndef FOX_OPTIONS
#define FOX_OPTIONS

/*
in argument names:
i = int
d = double
b = buffer
c = char
s = string

else: longer names
*/

#define FOX_VERSION "0.0.2"

//0:no fs
//1:fs
#define FILESYSTEM 1

//0:VGA
//1:VESA
#define GRAPHICS 0

//multitasking
#define MULTITASKING 1

//enable serial debugging (qemu console)
#define SERIAL 1


#define ATA_DEBUG 0
#define PROCESS_DEBUG 1

#endif