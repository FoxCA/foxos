
#include <kernel.h>

/* These are own ISRs that point to our special IRQ handler
*  instead of the regular 'fault_handler' function */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

/*
 * Remaps the IRQs so they are usable.
 */
void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

/*
 * Installs the IRQs.
 */
void irq_install()
{
    
    irq_remap();

    idt_set_entry(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_entry(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_entry(34, (unsigned)irq2, 0x08, 0x8E);
    idt_set_entry(35, (unsigned)irq3, 0x08, 0x8E);
    idt_set_entry(36, (unsigned)irq4, 0x08, 0x8E);
    idt_set_entry(37, (unsigned)irq5, 0x08, 0x8E);
    idt_set_entry(38, (unsigned)irq6, 0x08, 0x8E);
    idt_set_entry(39, (unsigned)irq7, 0x08, 0x8E);

    idt_set_entry(40, (unsigned)irq8, 0x08, 0x8E);
    idt_set_entry(41, (unsigned)irq9, 0x08, 0x8E);
    idt_set_entry(42, (unsigned)irq10, 0x08, 0x8E);
    idt_set_entry(43, (unsigned)irq11, 0x08, 0x8E);
    idt_set_entry(44, (unsigned)irq12, 0x08, 0x8E);
    idt_set_entry(45, (unsigned)irq13, 0x08, 0x8E);
    idt_set_entry(46, (unsigned)irq14, 0x08, 0x8E);
    idt_set_entry(47, (unsigned)irq15, 0x08, 0x8E);
    // idt_set_entry(128, (uint32_t)exception128, 0x08, 0x8E);
}

/*
 * This is the default IRQ handler which calls the child interrupt handlers.
 */
void irq_handler(struct regs *r)
{
    void (*handler)(struct regs *r);

    /* Find out if there is custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);
}
