
#ifndef KHEAPS_H_
#define KHEAPS_H_

#include <system.h>

uint32_t kmalloc_a(uint32_t sz);
uint32_t kmalloc_p(uint32_t sz, uint32_t* phys);
uint32_t kmalloc_ap(uint32_t sz, uint32_t* phys);
uint32_t kmalloc(uint32_t sz, int align, uint32_t* phys);

#endif // KHEAPS_H_