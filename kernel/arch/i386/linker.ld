ENTRY(_start)

SECTIONS {
	. = 2M; /* Start offset*/

	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot) /* Start with multiboot to get recognized*/
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K){
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K){
		*(COMMON)
		*(.bss)
	}
}
