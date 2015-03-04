#include <string.h>

int strcmp(const char* aptr, const char* bptr)
{
    while(*aptr && (*aptr == *bptr))
        ++aptr,++bptr;
    return (*aptr < *bptr) ? -1 : *aptr > *bptr;
}
