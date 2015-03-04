#include <errno.h>
#include <stdint.h>
#include <stdio.h>
/*
char* heap_ptr;

char* ksbrk(intptr_t nbytes)
{
    extern int stack_bottom;
    extern int stack_top;
    char* base;

    if(!heap_ptr)
        heap_ptr = (char*)&stack_top;
    base = heap_ptr;
    heap_ptr += nbytes;

    return base;
}
*/

/////

#define PAGESIZE 4096

#define RAM_MAXPAGE 0x100000
#define KERN_HEAP_LIM 0x40000000
#define USER_OFFSET 0x40000000
#define PG_PRESENT 0x00000001
#define PG_WRITE 0x00000002

#define set_page_frame_used(page) \
    mem_bitmap[(uintptr_t)page / 8] |= 1 << ((uintptr_t)page % 8)

struct kmalloc_header {
    unsigned long size:31;
    unsigned long used:1;
} __attribute__ ((packed));

char* kern_heap;

// XXX
//uint8_t mem_bitmap[RAM_MAXPAGE / 8];
uint8_t* mem_bitmap;

char* get_page_frame(void)
{
    uintptr_t byte, bit;
    uintptr_t page = -1;

    for (byte = 0; byte < RAM_MAXPAGE / 8; byte++)
        if (mem_bitmap[byte] != 0xFF)
            for (bit = 0; bit < 8; bit++)
                if (!(mem_bitmap[byte] & (1 << bit))) {
                    page = 8 * byte + bit;
                    set_page_frame_used(page);
                    return (char*)(page * PAGESIZE);
                }
    return (char*)-1;
}

int pd0_add_page(char* v_addr, char* p_addr, int flags)
{
    uint16_t* pde;
    uint16_t* pte;

    if (v_addr > (char*)KERN_HEAP_LIM) {
        printf("ERROR: pd0_add_page(): %d is not in kernel space!\n",
               (uintptr_t)v_addr);
        return 0;
    }

    pde = (uint16_t*)(0xFFFFF000 | (((uintptr_t)v_addr & 0xFFC00000) >> 20));
    if ((*pde & PG_PRESENT) == 0) {
//        printf("pd0_add_page: UNKNOWN ERROR!");
    }

    pte = (uint16_t*)(0xFFC00000 | (((uintptr_t)v_addr & 0xFFFFF000) >> 10));
    *pte = ((uintptr_t)p_addr) | (PG_PRESENT | PG_WRITE | flags);
    set_page_frame_used(p_addr);
    return 0;
}

void* ksbrk(intptr_t n)
{
//    printf("ksbrk(%d) ", n);
    struct kmalloc_header *chunk;
    char *p_addr;
    int i;

    if ((kern_heap + (n * PAGESIZE)) > (char*)KERN_HEAP_LIM) {
        printf("PANIC: ksbrk(): no virtual memory left for kernel heap !\n");
        return (char *) -1;
    }

    chunk = (struct kmalloc_header *) kern_heap;

    for (i = 0; i < n; i++) {
        p_addr = get_page_frame();
        if ((uintptr_t)p_addr < 0) {
            printf("PANIC: ksbrk(): no free page frame available !\n");
            return (char *) -1;
        }

        pd0_add_page(kern_heap, p_addr, 0);

        kern_heap += PAGESIZE;
    }

    chunk->size = PAGESIZE * n;
    chunk->used = 0;

    return chunk;
}
