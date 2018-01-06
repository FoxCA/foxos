#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>
#include <stdbool.h>

void	memory_init();
void *	memory_alloc_page( bool zeroit );
void	memory_free_page( void *addr );

#endif
