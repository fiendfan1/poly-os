#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);
double strtod(const char*, char**);
long strtol(const char*, char**, int);

void* malloc(size_t);
void free(void*);
void* realloc(void*, size_t);

// TODO
#define exit(E)

#ifdef __cplusplus
}
#endif

#endif
