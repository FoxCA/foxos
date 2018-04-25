#ifndef MALLOC_h
#define MALLOC_h
extern void *malloc(size_t size);
extern void free(void *mem);
extern void memory_init(void *mem, size_t size);
#endif