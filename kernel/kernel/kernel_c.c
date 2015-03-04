#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/alloc.h>
#include <kernel/gdt.h>

extern int stack_bottom;
extern int stack_top;

void heap_initialize(void)
{
//    memory_init((void*)KERN_HEAP, 1310720);
    memory_init((void*)&stack_top, 1310720);
}

void kernel_early(void)
{
    gdt_install();
    terminal_initialize();
    heap_initialize();
}

void modify_var(int* var, size_t len)
{
    for(size_t i = 0; i < len; ++i)
        *(var+i) = ((int)'a') + i;
}

void display_arr(int* arr, size_t len)
{
    for(size_t i = 0; i < len; ++i)
        printf("%c", (char)(*(arr + i)));
}

void test_heap(const int length)
{
#define SIZE 10
    void* ptrs[length];
    for(int i = 0; i < length; ++i)
    {
        ptrs[i] = malloc(SIZE);
        if(i == 0 || i == length-1)
            printf("Heap ptr #%d: << %d >> ", i, (uintptr_t)ptrs[i]);
    }
    for(int i = 0; i < length; ++i)
        free(ptrs[i]);
}

void kernel_main(void)
{
    test_heap(30000);
    test_heap(30000);

    /*
    uintptr_t bot_addr = (uintptr_t)&stack_bottom;
    uintptr_t top_addr = (uintptr_t)&stack_top;
    printf("stack_bottom: %d, stack_top: %d, diff: %d #\n", bot_addr, top_addr, top_addr - bot_addr);

    test_heap();

    int on_stack = 12;
    int* stack_addr = &on_stack;

    printf("Stack: # %d #\n", (uintptr_t)stack_addr);

    int* heap_addr = malloc(20);
    int* heap_addr2 = malloc(20);
    printf("Heap: # %d # %d #\n", (uintptr_t)heap_addr, (uintptr_t)heap_addr2);
*/
}
