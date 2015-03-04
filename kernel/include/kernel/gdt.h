#ifndef GDT_H
#define GDT_H

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void gdt_flush(uint32_t);

struct gdt_entry gdt[3];
struct gdt_ptr kgdtr;

//void gdt_flush(void);
void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);
void gdt_install(void);

#endif
