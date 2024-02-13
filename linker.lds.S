ENTRY(start16)
OUTPUT(binary)

SECTIONS
{
	. = 0x0;
	__kernel_16_start = .;

	.data16 : {
		__data16_start = .;
		start16.o(.data16)
		__data16_end = .;
	}

	.text16 : {
		__text16_start = .;
        start16.o(.text16);
		__text16_end = .;
    }

	__kernel_16_end = .;

	. = 0x100000;
	__kernel_32_start = .;

	.data32 : ALIGN(4096) {
		__data32_start = .;
		start32.o(.data32)
		__data32_end = .;
	}

	.text32 : ALIGN(4096) {
		__text32_start = .;
		start32.o(.text32)
		__text32_end = .;
	}

	__kernel_32_end = .;

	. = 0xffff800000000000 + __kernel_32_end;
	__kernel_64_start = .;

	.data64 : AT(__kernel_32_end) ALIGN(4096) {
		__data64_start = .;
		*(.data64)
		*(.data)
		__data64_end = .;
	}

	.bss64  : ALIGN(4096) {
		__bss64_start = .;
		*(.bss64)
		*(.bss)
		__bss64_end = .;
	}

	.text64 : ALIGN(4096)  {
		__text64_start = .;
		*(.text64)
		*(.text)
		__text64_end = .;
	}

	__kernel_64_end = .;

	__kernel_size__ = (__kernel_16_end - __kernel_16_start) +
					  (__kernel_32_end - __kernel_32_start) +
					  (__kernel_64_end - __kernel_64_start);
	/DISCARD/ : {
		*(.note.gnu.property)
	}
}
