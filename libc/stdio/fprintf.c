#include <stdio.h>
#include <stdarg.h>

int vfprintf(FILE* file, const char* format, va_list va)
{
    return printf(format, va);
}

int fprintf(FILE* file, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int ret = vfprintf(file, format, va);
    va_end(va);
    return ret;
}
