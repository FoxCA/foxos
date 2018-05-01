#include <kernel.h>

idt_entry_t idt_entries[NUM_IDT_ENTRIES];
idt_ptr_t idt_ptr;

/*
 * Initializes the Interrupt Descriptor Table.
 * The IDT is similar to the GDT, except that it handles interrupts.
 * You won't need this, ever! Messing with this will probably mess up everything else, too.
 */
void idt_init() {
    memset(idt_entries, 0, sizeof(idt_entries));
    idt_ptr.base = (uint32_t)idt_entries;
    idt_ptr.limit = sizeof(idt_entries) - 1;
    pic_init();

    irq_install();

    idt_flush((uint32_t)&(idt_ptr));
    asm volatile("sti");
}

/*
 * Sets an entry in the IDT.
 */
void idt_set_entry(int index, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entry_t * this = &idt_entries[index];
    
    this->base_lo = base & 0xFFFF;
    this->base_hi = (base >> 16) & 0xFFFF;
    
    this->always0 = 0;
    
    this->sel = sel;
    
    this->flags = flags | 0x60;
}