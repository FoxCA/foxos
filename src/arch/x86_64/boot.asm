extern start64
global start
global _isr0
global _isr1
global _isr2
global _isr3    
global _isr4    
global _isr5    
global _isr6    
global _isr7    
global _isr8    
global _isr9    
global _isr10   
global _isr11   
global _isr12   
global _isr13   
global _isr14   
global _isr15   
global _isr16   
global _isr17   
global _isr18   
global _isr19   
global _isr20   
global _isr21   
global _isr22   
global _isr23   
global _isr24   
global _isr25   
global _isr26   
global _isr27   
global _isr28   
global _isr29   
global _isr30   
global _isr31



section .text
bits 32
start:
    mov esp, stack_top

    call check_multiboot
    call check_cpuid
    call check_long_mode

    call set_up_page_tables ; new
    call enable_paging     ; new

    ; load the 64-bit GDT
    lgdt [gdt64.pointer]
    jmp gdt64.code:start64

    ; print `OK` to screen
    ; should never happen because long_mode is started
    mov dword [0xb8000], 0x2f4b2f4f
    hlt


idtsetup:
    ;  0: Divide By Zero Exception
    _isr0:
        cli
        push byte 0    ; A normal ISR stub that pops a dummy error code to keep a  uniform stack frame
        push byte 0
        jmp isr_common_stub

    ;  1: Debug Exception
    _isr1:
        cli
        push byte 0
        push byte 1
        jmp isr_common_stub

    ;  2: Non Maskable Interrupt Exception
    _isr2:
        cli
        push byte 0
        push byte 2
        jmp isr_common_stub

    ;  3: Breakpoint Exception
    _isr3:
        cli
        push byte 0
        push byte 3
        jmp isr_common_stub

    ;  4: Into Detected Overflow Exception
    _isr4:
        cli
        push byte 0
        push byte 4
        jmp isr_common_stub

    ;  5: Out of Bounds Exception
    _isr5:
        cli
        push byte 0
        push byte 5
        jmp isr_common_stub

    ;  6: Invalid Opcode Exception
    _isr6:
        cli
        push byte 0
        push byte 6
        jmp isr_common_stub

    ;  7: No Coprocessor Exception
    _isr7:
        cli
        push byte 0
        push byte 7
        jmp isr_common_stub
    
    ;  8: Double Fault Exception
    _isr8:
        cli
        push byte 8         ; Note that we DON'T push a value on the stack in this one!
                            ; It pushes one already! Use this type of stub for exceptions
                            ; that pop error codes!
        jmp isr_common_stub

    ;  9: Coprocessor Segment Overrun Exception
    _isr9:
        cli
        push byte 0
        push byte 9
        jmp isr_common_stub     

    ;  10: Bad TSS Exception
    _isr10:
        cli
        push byte 10        ; Note that we DON'T push a value on the stack in this one!
                            ; It pushes one already! Use this type of stub for exceptions
                            ; that pop error codes!
        jmp isr_common_stub ;  8: Double Fault Exception
    
    ;  11: Segment Not Present Exception
    _isr11:
        cli
        push byte 11        ; Note that we DON'T push a value on the stack in this one!
                            ; It pushes one already! Use this type of stub for exceptions
                            ; that pop error codes!
        jmp isr_common_stub

    ;  12: Stack Fault Exception
    _isr12:
        cli
        push byte 12        ; Note that we DON'T push a value on the stack in this one!
                            ; It pushes one already! Use this type of stub for exceptions
                            ; that pop error codes!
        jmp isr_common_stub

    ;  13: General Protection Fault Exception
    _isr13:
        cli
        push byte 13        ; Note that we DON'T push a value on the stack in this one!
                            ; It pushes one already! Use this type of stub for exceptions
                            ; that pop error codes!
        jmp isr_common_stub

    ;  14: Page Fault Exception
    _isr14:
        cli
        push byte 14        ; Note that we DON'T push a value on the stack in this one!
                            ; It pushes one already! Use this type of stub for exceptions
                            ; that pop error codes!
        jmp isr_common_stub

    ;  15: Unknown Interrupt Exception
    _isr15:
        cli
        push byte 0
        push byte 15
        jmp isr_common_stub

    ;  16: Coprocessor Fault Exception
    _isr16:
        cli
        push byte 0
        push byte 16
        jmp isr_common_stub

    ;  17: Alignment Check Exception (486+)
    _isr17:
        cli
        push byte 0
        push byte 17
        jmp isr_common_stub

    ;  18: Machine Check Exception (Pentium/586+)
    _isr18:
        cli
        push byte 0
        push byte 18
        jmp isr_common_stub

    ;  19: Reserved Exception
    _isr19:
        cli
        push byte 0
        push byte 19
        jmp isr_common_stub

    ;  20: Reserved Exception
    _isr20:
        cli
        push byte 0
        push byte 20
        jmp isr_common_stub

    ;  21: Reserved Exception
    _isr21:
        cli
        push byte 0
        push byte 21
        jmp isr_common_stub

    ;  22: Reserved Exception
    _isr22:
        cli
        push byte 0
        push byte 22
        jmp isr_common_stub

    ;  23: Reserved Exception
    _isr23:
        cli
        push byte 0
        push byte 23
        jmp isr_common_stub

    ;  24: Reserved Exception
    _isr24:
        cli
        push byte 0
        push byte 24
        jmp isr_common_stub

    ;  25: Reserved Exception
    _isr25:
        cli
        push byte 0
        push byte 25
        jmp isr_common_stub

    ;  256: Reserved Exception
    _isr26:
        cli
        push byte 0
        push byte 26
        jmp isr_common_stub

    ;  27: Reserved Exception
    _isr27:
        cli
        push byte 0
        push byte 27
        jmp isr_common_stub

    ;  28: Reserved Exception
    _isr28:
        cli
        push byte 0
        push byte 28
        jmp isr_common_stub

    ;  29: Reserved Exception
    _isr29:
        cli
        push byte 0
        push byte 29
        jmp isr_common_stub

    ;  30: Reserved Exception
    _isr30:
        cli
        push byte 0
        push byte 30
        jmp isr_common_stub

    ;  31: Reserved Exception
    _isr31:
        cli
        push byte 0
        push byte 31
        jmp isr_common_stub

    pushall:
        push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi
        push ebp
        push esp
        ret

    popall:
        pop esp
        pop ebp
        pop edi
        pop esi
        pop edx
        pop ecx
        pop ebx
        pop eax
        ret

    extern _fault_handler
    isr_common_stub:
        call pushall
        push ds
        push es
        push fs
        push gs
        mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov eax, esp   ; Push us the stack
        push eax
        mov eax, _fault_handler
        call eax       ; A special call, preserves the 'eip' register
        pop eax
        pop gs
        pop fs
        pop es
        pop ds
        call popall
        add esp, 8     ; Cleans up the pushed error code and pushed ISR number
        iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret

check_multiboot:
	cmp eax, 0x36d76289
	jne .no_multiboot
	ret
.no_multiboot:
	mov al, "0"
	jmp error

error:
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

check_cpuid:
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.

    ; Copy FLAGS in to EAX via stack
    pushfd
    pop eax

    ; Copy to ECX as well for comparing later on
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
    push eax
    popfd

    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
    push ecx
    popfd

    ; Compare EAX and ECX. If they are equal then that means the bit
    ; wasn't flipped, and CPUID isn't supported.
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp error

check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    ret
.no_long_mode:
    mov al, "2"
    jmp error


set_up_page_tables:
	; map first P4 entry to P3 table
	mov eax, p3_table
	or eax, 0b11 ; present + writable
	mov [p4_table], eax

	; map first P3 entry to P2 table
	mov eax, p2_table
	or eax, 0b11 ; present + writable
	mov [p3_table], eax

    ; map each P2 entry to a huge 2MiB page
    mov ecx, 0         ; counter variable

	.map_p2_table:
		; map ecx-th P2 entry to a huge page that starts at address 2MiB*ecx
		mov eax, 0x200000  ; 2MiB
		mul ecx            ; start address of ecx-th page
		or eax, 0b10000011 ; present + writable + huge
		mov [p2_table + ecx * 8], eax ; map ecx-th entry

		inc ecx            ; increase counter
		cmp ecx, 512       ; if counter == 512, the whole P2 table is mapped
		jne .map_p2_table  ; else map the next entry

		ret

enable_paging:
    ; load P4 to cr3 register (cpu uses this to access the P4 table)
    mov eax, p4_table
    mov cr3, eax

    ; enable PAE-flag in cr4 (Physical Address Extension)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit in the EFER MSR (model specific register)
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

section .bss
	align 4096
	p4_table:
		resb 4096
	p3_table:
		resb 4096
	p2_table:
		resb 4096
	stack_bottom:
		resb 4096 * 4
	stack_top:

section .rodata
gdt64:
	dq 0 ; zero entry
	dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
	.code: equ $ - gdt64 ; new
		dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment

	.pointer:
		dw $ - gdt64 - 1
		dq gdt64