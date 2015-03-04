#include <time.h>

clock_t clock(void) {
    return 0;
}

time_t time(time_t* tloc) {
    *tloc = 0;
    return 0;
}
