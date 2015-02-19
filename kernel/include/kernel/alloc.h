#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

void* kmalloc(size_t);
void kfree(void*);

void memory_init(void*, size_t);

#endif
