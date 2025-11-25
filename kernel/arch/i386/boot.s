; Multiboot constants
MBALIGN equ 1 << 0 ; Align on page boundaries
MEMINFO equ 1 << 1 ; Provide momory map

MBFLAGS equ MBALIGN | MEMINFO ; Flags

MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC+MBFLAGS) ; Adding checksum and the above must yield 0 (zero)

section .multiboot
align 4 ; align to 32 bits
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM


section .bss
align 16
stack_bottom: resb 16384
stack_top:

section .text:
extern boottime_init
global _start
_start:

	; Setup stack
	mov esp, stack_top

	push eax
	push ebx
	call boottime_init

	; Things to load here later:
	; GDT
	; Paging
	; Floating Arithmetic
	; CPU extensions

	; Make sure the stack is 16 byte aligned before calling main (since we need to comply with the  ABI)

	extern k_main
	call k_main

	; Why are we returning? Odd... loop forever
	cli
.hang: hlt
	jmp .hang
.end:
