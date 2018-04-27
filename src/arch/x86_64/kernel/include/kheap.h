
// #ifndef KHEAPS_H
// #define KHEAPS_H

// #include <system.h>

// uint32_t kmalloc_a(uint32_t sz);
// uint32_t kmalloc_p(uint32_t sz, uint32_t* phys);
// uint32_t kmalloc_ap(uint32_t sz, uint32_t* phys);
// uint32_t kmalloc(uint32_t sz, int align, uint32_t* phys);

// #endif // KHEAPS_H

#ifndef KHEAP_H
#define KHEAP_H

#include <system.h>
#include <types.h>

#define KHEAP_START         (void*)0xC0400000
#define KHEAP_INITIAL_SIZE  48 * M
#define KHEAP_MAX_ADDRESS   (void*)0xCFFFFFFF
#define HEAP_MIN_SIZE       4 * M

#define PAGE_SIZE 4096
#define OVERHEAD (sizeof(struct Block) + sizeof(uint32_t))


struct Block
{
    uint32_t size;
    struct Block * prev;
    struct Block * next;
};

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys);

void * kmalloc_cont(uint32_t sz, int align, uint32_t *phys);

void * kmalloc_a(uint32_t sz);

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);

void * kmalloc(uint32_t sz);

void kfree(void *p);

void * krealloc(void * ptr, uint32_t size);

void db_print();

int doesItFit(struct Block * n, uint32_t size);

void setFree(uint32_t *size, int x);

void removeNodeFromFreelist(struct Block * x);

void addNodeToFreelist(struct Block * x);

int isBetter(struct Block * node1, struct Block * node2);

struct Block * bestfit(uint32_t size);

struct Block * getPrevBlock(struct Block * n);

struct Block * getNextBlock(struct Block * n);

uint32_t getRealSize(uint32_t size);

uint32_t getSbrkSize(uint32_t size);

int isFree(struct Block * n);

int isEnd(struct Block * n);

void kheap_init(void * start, void * end, void * max);

void *malloc(uint32_t size);

void free(void * ptr);

void *kcalloc(uint32_t num, uint32_t size);

void *realloc(void *ptr, uint32_t size);

#endif