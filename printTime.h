#include <stdio.h>
#include <sys/time.h>

void printTime(timeval t0, timeval t1){
        t1.tv_sec -= t0.tv_sec;
        if (t1.tv_usec < t0.tv_usec) {
                t1.tv_sec -= 1;
                t1.tv_usec += 1000000 - t0.tv_usec;
        } else {
                t1.tv_usec -= t0.tv_usec;
        }
        printf("%ld.%06d sec\n\n", t1.tv_sec, t1.tv_usec);
}