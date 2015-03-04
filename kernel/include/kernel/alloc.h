#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

void* kmalloc(size_t);
void kfree(void*);
void* krealloc(void*, size_t);

void memory_init(void*, size_t);

#endif
