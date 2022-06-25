/* macros.h: PQSH Macros */

#ifndef PQSH_MACROS_H
#define PQSH_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Logging */

#ifdef DEBUG
#define debug(M, ...) \
    fprintf(stderr, "[%09lu] DEBUG %s:%d:%s: " M "\n", time(NULL), __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define debug(M, ...)
#endif

#define info(M, ...) \
    fprintf(stderr, "[%09lu] INFO  " M "\n", time(NULL), ##__VA_ARGS__)

#define error(M, ...) \
    fprintf(stderr, "[%09lu] ERROR " M "\n", time(NULL), ##__VA_ARGS__)

/* Utilities */

#define chomp(s)            if (strlen(s)) { s[strlen(s) - 1] = 0; }
#define min(a, b)           ((a) < (b) ? (a) : (b))
#define streq(a, b)         (strcmp(a, b) == 0)

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
