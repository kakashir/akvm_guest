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

#endif
