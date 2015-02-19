#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/alloc.h>

void heap_initialize(void)
{
    memory_init((void*)(0x100000 + 16385), 16384);
}

void kernel_early(void)
{
    heap_initialize();
	terminal_initialize();
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

void kernel_main(void)
{
    int* var = malloc(sizeof(int)*20);
    modify_var(var, 20);
    display_arr(var, 20);

    free(var);
}
