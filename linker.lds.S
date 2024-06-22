#include "config.h"

ENTRY(start16)
OUTPUT(binary)
OUTPUT_FORMAT(KERNEL_OUTPUT_FORMAT)

SECTIONS
{
	. = KERNEL_16_LOAD_PHY_ADDR;
	__kernel_16_start = .;

	.data16 : ALIGN(4096) {
		__data16_start = .;
		*(.data16)
		__data16_end = .;
	}

	.text16 : ALIGN(4096) {
		__text16_start = .;
		*(.text16);
		__text16_end = .;
    }

	__kernel_16_end = .;

	. = KERNEL_32_LOAD_PHY_ADDR;
	__kernel_32_start = .;

	.data32 : ALIGN(4096) {
		__data32_start = .;
		*(.data32)
		__data32_end = .;
	}

	.text32 : ALIGN(4096) {
		__text32_start = .;
		*(.text32)
		__text32_end = .;
	}

	__kernel_32_end = .;

	.text64_low : ALIGN(4096) {
		__kernel_64_start = .;
		__kernel_64_low_start = .;
		*(.text64_low)
	}

	__kernel_64_low_end = .;
	__kernel_64_high_start = .;

	. = KERNEL_LOAD_VA_ADDR + __kernel_64_low_end;
	.data64 : AT(ALIGN(__kernel_64_low_end, 4096)) ALIGN(4096) {
		__data64_start = .;
		*(.data64)
		*(.data)
		__data64_end = .;
	}

	.bss64  : ALIGN(1) {
		__bss64_start = .;
		*(.bss64)
		*(.bss)
		__bss64_end = .;
	}

	.text64 : ALIGN(8)  {
		__text64_start = .;
		*(.text64)
		*(.text)
		__text64_end = .;
	}

	__kernel_64_high_end = . - KERNEL_LOAD_VA_ADDR;
	__kernel_64_end = .;
	__kernel_size__ = (__kernel_16_end - __kernel_16_start) +
					  (__kernel_32_end - __kernel_32_start) +
					  (__kernel_64_end - __kernel_64_start);
	/DISCARD/ : {
		*(.note.gnu.property)
		*(.note.gnu.build-id)
	}
}
