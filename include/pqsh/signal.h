/* signal.h: PQSH Signal Handlers */

#ifndef PQSH_SIGNAL_H
#define PQSH_SIGNAL_H

#include <signal.h>
#include <stdbool.h>

#ifndef GNU_SOURCE
typedef void (*sighandler_t)(int);
#endif

/* Functions */

bool    signal_register(int signum, int flags, sighandler_t handler);

/* Handlers */

void	sigalrm_handler(int signum);

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
