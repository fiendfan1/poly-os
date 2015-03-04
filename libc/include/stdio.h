#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

// XXX: DUMMY
typedef int FILE;

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

int sprintf(char*, const char*, ...);

FILE* stdout;
FILE* stderr;
FILE* stdin;

int fprintf(FILE*, const char*, ...);

#ifdef __cplusplus
}
#endif

#endif
