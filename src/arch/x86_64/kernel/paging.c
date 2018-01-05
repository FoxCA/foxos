// #include <paging.h>
// #include <stdio.h>

// unsigned long *page_directory = (unsigned long *) 0x9C000;
// unsigned long *page_table = (unsigned long *) 0x9D000;

// void start_paging(){
// 	unsigned long address=0; // holds the physical address of where a page is
// 	unsigned int i;

// 	for(i=0; i<1024; i++){
// 		page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
// 		address = address + 4096; // 4096 = 4kb
// 	};
// u
// 	page_directory[0] = page_table; // attribute set to: supervisor level, read/write, present(011 in binary)
// 	page_directory[0] = page_directory[0] | 3;

// 	for(i=1; i<1024; i++){
// 		page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)
// 	};

// 	write_cr3(page_directory); // put that page directory address into CR3
// 	write_cr0(read_cr0() | 0x80000000); // set the paging bit in CR0 to 1
// }


#include <paging.h>
#include <memory.h>
#include <string.h>
// #include "kernelcore.h"

#define ENTRIES_PER_TABLE (PAGE_SIZE/4)

struct pageentry {
	unsigned present:1;	// 1 = present
	unsigned readwrite:1;	// 1 = writable
	unsigned user:1;	// 1 = user mode
	unsigned writethrough:1; // 1 = write through

	unsigned nocache:1;	// 1 = no caching
	unsigned accessed:1;	// 1 = accessed
	unsigned dirty:1;	// 1 = dirty
	unsigned pagesize:1;	// leave to zero

	unsigned globalpage:1;	// 1 if not to be flushed
	unsigned avail:3;

	unsigned addr:20;
};

struct pagetable {
	struct pageentry entry[ENTRIES_PER_TABLE];
};

struct pagetable * pagetable_create()
{
	return memory_alloc_page(1);
}

void pagetable_init( struct pagetable *p )
{
	unsigned i,stop;
	stop = total_memory*1024*1024;
	for(i=0;i<stop;i+=PAGE_SIZE) {
		pagetable_map(p,i,i,PAGE_FLAG_KERNEL|PAGE_FLAG_READWRITE);
	}
	stop = (unsigned)video_buffer+video_xres*video_yres*3;
	for(i=(unsigned)video_buffer;i<=stop;i+=PAGE_SIZE) {
		pagetable_map(p,i,i,PAGE_FLAG_KERNEL|PAGE_FLAG_READWRITE);
	}
}

int pagetable_getmap( struct pagetable *p, unsigned vaddr, unsigned *paddr )
{
	struct pagetable *q;
	struct pageentry *e;

	unsigned a = vaddr>>22;
	unsigned b = (vaddr>>12) & 0x3ff;

	e = &p->entry[a];
	if(!e->present) return 0;

	q = (struct pagetable*) (e->addr << 12);

	e = &q->entry[b];
	if(!e->present) return 0;

	*paddr = e->addr << 12;

	return 1;
}

int pagetable_map( struct pagetable *p, unsigned vaddr, unsigned paddr, int flags )
{
	struct pagetable *q;
	struct pageentry *e;

	unsigned a = vaddr>>22;
	unsigned b = (vaddr>>12) & 0x3ff;

	if(flags&PAGE_FLAG_ALLOC) {
		paddr = (unsigned) memory_alloc_page(0);
		if(!paddr) return 0;
	}

	e = &p->entry[a];

	if(!e->present) {
		q = pagetable_create();
		if(!q) return 0;
		e->present = 1;
		e->readwrite = 1;
		e->user = (flags&PAGE_FLAG_KERNEL) ? 0 : 1;
		e->writethrough = 0;
		e->nocache = 0;
		e->accessed = 0;
		e->dirty = 0;
		e->pagesize = 0;
		e->globalpage = (flags&PAGE_FLAG_KERNEL) ? 1 : 0;
		e->avail = 0;
		e->addr = (((unsigned)q) >> 12);
	} else {
		q = (struct pagetable*) (((unsigned)e->addr) << 12);
	}


	e = &q->entry[b];

	e->present = 1;
	e->readwrite = (flags&PAGE_FLAG_READWRITE) ? 1 : 0;
	e->user = (flags&PAGE_FLAG_KERNEL) ? 0 : 1;
	e->writethrough = 0;
	e->nocache = 0;
	e->accessed = 0;
	e->dirty = 0;
	e->pagesize = 0;
	e->globalpage = !e->user;
	e->avail = (flags&PAGE_FLAG_ALLOC) ? 1 : 0;
	e->addr = (paddr >> 12);

	return 1;
}

void pagetable_unmap( struct pagetable *p, unsigned vaddr )
{
	struct pagetable *q;
	struct pageentry *e;

	unsigned a = vaddr>>22;
	unsigned b = vaddr>>12 & 0x3ff;

	e = &p->entry[a];
	if(e->present) {
		q = (struct pagetable *)(e->addr << 12);
		e = &q->entry[b];
		e->present = 0;
	}
}

void pagetable_delete( struct pagetable *p )
{
	unsigned i,j;

	struct pageentry *e;
	struct pagetable *q;

	for(i=0;i<ENTRIES_PER_TABLE;i++) {
		e = &p->entry[i];
		if(e->present) {
			q = (struct pagetable *) (e->addr<<12);
			for(j=0;j<ENTRIES_PER_TABLE;j++) {
				e = &q->entry[i];
				if(e->present && e->avail) {
					void *paddr;
					paddr = (void *) (e->addr<<12);
					memory_free_page(paddr);
				}
			}
			memory_free_page(q);
		}
	}
}

void pagetable_alloc( struct pagetable *p, unsigned vaddr, unsigned length, int flags )
{
	unsigned npages = length/PAGE_SIZE;

	if(length%PAGE_SIZE) npages++;

	vaddr &= 0xfffff000;

	while(npages>0) {
		unsigned paddr;
		if(!pagetable_getmap(p,vaddr,&paddr)) {
			pagetable_map(p,vaddr,0,flags|PAGE_FLAG_ALLOC);
		}
		vaddr += PAGE_SIZE;
		npages--;
	}
}

struct pagetable * pagetable_load( struct pagetable *p )
{
	struct pagetable *oldp;
	asm("mov %%cr3, %0" : "=r" (oldp));
	asm("mov %0, %%cr3" :: "r" (p));
	return oldp;
}

void pagetable_refresh()
{
	asm("mov %cr3, %eax");
	asm("mov %eax, %cr3");
}

void pagetable_enable()
{
	asm("movl %cr0, %eax");
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");
}

void pagetable_copy( struct pagetable *sp, unsigned saddr, struct pagetable *tp, unsigned taddr, unsigned length );
