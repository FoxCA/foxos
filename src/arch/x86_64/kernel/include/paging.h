
#ifndef PAGING_H
#define PAGING_H

#include <system.h>
#include <types.h>

typedef struct page
{
	uint32_t present	: 1;	// page present in memory
	uint32_t rw			: 1;	// read-only if 0, rw else
	uint32_t user		: 1;	// supervisor if 0, user else
	uint32_t accessed	: 1;	// accessed since last refresh?
	uint32_t dirty		: 1;	// written since last refresh?
	uint32_t unused		: 7;
	uint32_t frame		: 20;	// frame address (shifted left 12 bits)
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	page_table_t* tables[1024];
	uint32_t tablesPhysical[1024];
	uint32_t physicalAddr;
} page_directory_t;

void initialize_paging();

// cause specified page directory to be loaded to CR3 register
void switch_page_directory(page_directory_t* new);

page_t* get_page(uint32_t address, int make, page_directory_t* dir);

void page_fault(regs * regs_t);

#endif