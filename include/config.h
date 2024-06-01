#ifndef __CONFIG_H
#define __CONFIG_H

#define KERNEL_16_LOAD_PHY_ADDR  0x0
#define KERNEL_32_LOAD_PHY_ADDR  0x100000

/*  Follow gcc's -mcmodel=kernel */
#define KERNEL_LOAD_VA_ADDR   0xffffffff80000000
#define KERNEL_SIZE  (0xffffffffffffffffULL - KERNEL_LOAD_VA_ADDR + 1)

#define KERNEL_OUTPUT_FORMAT "elf64-x86-64"

#define KERNEL_DS_64  0x8
#define KERNEL_CS_64  0x10
#define KERNEL_TSS_64 0x18

#define EXCEP_2_IST   1
#define EXCEP_8_IST   2
#define EXCEP_18_IST  3

/* nromal config start */

/*
  take care stack overflow when enlarge this,
  the kernel stack is small, i.e only 4K allocated.
 */
#define CONFIG_PRINT_BUF_SIZE 256
#define MAX_NR_PHY_MEM_RANGE 32

/* nromal config end */

/* platform fixed device start */

#define SERIAL_PORT_OUT 0xe0001

#define HWCFG_REG_TYPE 0xe0010
	#define HWCFG_MEMORY_INFO (1ULL)
#define HWCFG_REG_CTL 0xe0011
	#define HWCFG_CTL_NEXT_ITEM (1ULL)
#define HWCFG_REG_DATA_0 0xe0012
#define HWCFG_REG_DATA_1 0xe0013
#define HWCFG_REG_DATA_2 0xe0014
#define HWCFG_REG_DATA_3 0xe0015

#define HWCFG_MEMORY_TYPE_PHYSICAL (1ULL)


/* platform fixed device end */
#endif
