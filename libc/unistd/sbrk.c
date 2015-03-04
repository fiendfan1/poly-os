#include <kernel/alloc.h>

void* sbrk(int n)
{
    ksbrk(n);
}
