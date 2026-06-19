#ifndef PROFILE_H
#define PROFILE_H

#include <time.h>

// __VA_ARGS__ : It can be used for textual subsitution of the variable args passed to the macro
// it will simply copy and paste that as is anywhere else

// # : We can stringify the inputs provided to the macros be appending '#' as the prefix

#define measure_runtime_ms(FUNC, ...) { struct timespec start, end;\
    clock_gettime(CLOCK_REALTIME, &start);\
    FUNC(__VA_ARGS__);\
    clock_gettime(CLOCK_REALTIME, &end);\
    double elapsed_time = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec);\
    fprintf(stderr, "Runtime of " #FUNC "() : %lf ms\n", elapsed_time / 1e6);\
    }

#define measure_runtime_us(FUNC, ...) { struct timespec start, end;\
    clock_gettime(CLOCK_REALTIME, &start);\
    FUNC(__VA_ARGS__);\
    clock_gettime(CLOCK_REALTIME, &end);\
    double elapsed_time = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec);\
    fprintf(stderr, "Runtime of " #FUNC "() : %lf us\n", elapsed_time / 1e3);\
    }

#define measure_runtime_ns(FUNC, ...) { struct timespec start, end;\
    clock_gettime(CLOCK_REALTIME, &start);\
    FUNC(__VA_ARGS__);\
    clock_gettime(CLOCK_REALTIME, &end);\
    double elapsed_time = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec);\
    fprintf(stderr, "Runtime of " #FUNC "() : %lf ns\n", elapsed_time);\
    }

#endif
