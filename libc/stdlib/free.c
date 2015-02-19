#include <kernel/alloc.h>

void free(void* mem) {
    kfree(mem);
}
