#include <stdlib.h>
#include <kernel/alloc.h>

void* realloc(void* ptr, size_t size) {
    return krealloc(ptr, size);
}
