#include <paging.h>
#include <stdio.h>

unsigned long *page_directory = (unsigned long *) 0x9C000;
unsigned long *page_table = (unsigned long *) 0x9D000;

void start_paging(){
	unsigned long address=0; // holds the physical address of where a page is
	unsigned int i;

	for(i=0; i<1024; i++){
		page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
		address = address + 4096; // 4096 = 4kb
	};

	page_directory[0] = page_table; // attribute set to: supervisor level, read/write, present(011 in binary)
	page_directory[0] = page_directory[0] | 3;

	for(i=1; i<1024; i++){
		page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)
	};

	write_cr3(page_directory); // put that page directory address into CR3
	write_cr0(read_cr0() | 0x80000000); // set the paging bit in CR0 to 1
	printf("hey");
}

