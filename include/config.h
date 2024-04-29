#ifndef __CONFIG_H
#define __CONFIG_H

#define KERNEL_16_LOAD_PHY_ADDR  0x0
#define KERNEL_32_LOAD_PHY_ADDR  0x100000

/*  Follow gcc's -mcmodel=kernel */
#define KERNEL_64_LOAD_VA_ADDR   0xffffffff80000000
#define KERNEL_SIZE  (0xffffffffffffffffULL - KERNEL_64_LOAD_VA_ADDR + 1)

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

/* nromal config end */

/* platform fixed device start */

#define SERIAL_PORT_OUT 0xe0001

/* platform fixed device end */
#endif
