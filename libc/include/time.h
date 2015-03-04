#ifndef TIME_H
#define TIME_H

#define CLOCKS_PER_SEC 1000

typedef unsigned long time_t;
typedef unsigned long clock_t;

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

clock_t clock(void);
time_t time(time_t*);

#endif
