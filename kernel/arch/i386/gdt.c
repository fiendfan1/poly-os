#include <stdint.h>
#include <string.h>
#include <kernel/gdt.h>
/*
void gdt_flush(void)
{
    asm("lgdtl (kgdtr)");

    asm("movw $0x10, %ax    \n \
         movw %ax, %ds    \n \
         movw %ax, %es    \n \
         movw %ax, %fs    \n \
         movw %ax, %gs    \n");
}
*/

/*
void gdt_flush(void)
{
    asm("lgdtl (kgdtr)");

    asm("   movw $0x10, %ax    \n \
            movw %ax, %ds    \n \
            movw %ax, %es    \n \
            movw %ax, %fs    \n \
            movw %ax, %gs    \n \
            ljmp $0x08, $next    \n \
            next:        \n");
}
    */


void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit,
                           uint8_t access, uint8_t gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_install(void)
{
    // Setup the GDT pointer and limit
    kgdtr.limit = (sizeof(struct gdt_entry) * 3) - 1;
    kgdtr.base = (uint32_t)&gdt;

//    #define GRAN 0x0D
    #define GRAN 0xCF

    // Null descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    // Code
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, GRAN);
    // Data
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, GRAN);
    gdt_set_gate(3, 0, 0, 0x97, GRAN);
/*
    // Null descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    // Code
    gdt_set_gate(1, 0, 0xFFFFF, 0x9B, GRAN);
    // Data
    gdt_set_gate(2, 0, 0xFFFFF, 0x93, GRAN);
    // Stack
    gdt_set_gate(3, 0, 0, 0x97, GRAN);
*/
/*
    // Our NULL descriptor
    gdt_set_gate(0, 0, 0, 0, 0);

    // The second entry is our Code Segment. The base address
    // is 0, the limit is 4GBytes, it uses 4KByte granularity,
    // uses 32-bit opcodes, and is a Code Segment descriptor.
    // Please check the table above in the tutorial in order
    // to see exactly what each value means
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // The third entry is our Data Segment. It's EXACTLY the
    // same as our code segment, but the descriptor type in
    // this entry's access byte says it's a Data Segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
*/

    // Flush out the old GDT and install the new changes!
    //gdt_flush();
    gdt_flush((uint32_t)&kgdtr);
}
