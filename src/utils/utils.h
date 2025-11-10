#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define LOG(...) fprintf(stderr, __VA_ARGS__)

#if defined(__GNUC__) || defined(__clang__)
    #define FORCE_INLINE __attribute__((always_inline)) inline
#else
    #define FORCE_INLINE __forceinline
#endif

/* THIS MALLOCS A STRING, DONT FORGET TO FREE IT */
char *read_file(FILE *file);

#endif
