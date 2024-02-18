#ifndef __ENTRY_H
#define __ENTRY_H

void __idt_intr_one_start(void);
extern unsigned long idt_intr_entry_point_size;

void asm_excep_0(void);
void asm_excep_1(void);
void asm_excep_2(void);
void asm_excep_3(void);
void asm_excep_4(void);
void asm_excep_5(void);
void asm_excep_6(void);
void asm_excep_7(void);
void asm_excep_8(void);
void asm_excep_9(void);
void asm_excep_10(void);
void asm_excep_11(void);
void asm_excep_12(void);
void asm_excep_13(void);
void asm_excep_14(void);
void asm_excep_16(void);
void asm_excep_17(void);
void asm_excep_18(void);
void asm_excep_19(void);
void asm_excep_20(void);
void asm_excep_21(void);

long vm_service(unsigned long id, unsigned long arg0,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4);

struct idt64_desc;
void load_idt(struct idt64_desc *desc);

struct gdt64_desc;
void load_gdt(struct gdt64_desc *desc);
void flush_segment_cache(void);

#endif
