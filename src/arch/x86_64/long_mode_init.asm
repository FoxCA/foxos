global start64

section .text
bits 64
align 16


start64:

	extern main
    call main

    ; print `OKAY` to screen
    mov rax, 0x2f592f412f4b2f4f
    mov qword [0xb8000], rax
    hlt
