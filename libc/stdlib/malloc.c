//#include <stdint.h>
#include <kernel/alloc.h>

void* malloc(size_t size) {
    return kmalloc(size);
}
