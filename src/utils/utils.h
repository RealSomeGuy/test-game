#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define LOG(...) fprintf(stderr, __VA_ARGS__)

/* THIS MALLOCS A STRING, DONT FORGET TO FREE IT */
char *read_file(FILE *file);

#endif
