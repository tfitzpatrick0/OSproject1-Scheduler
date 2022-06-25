/* signal.c: PQSH Signal Handlers */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include "pqsh/signal.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Register signal handler with specified flags.
 * @param   signum      Signal number
 * @param   flags       Signal action flags
 * @param   handler     Signal handler
 * @return  Whether or not the registration was successful.
 **/
bool signal_register(int signum, int flags, sighandler_t handler) {
    struct sigaction action = {{0}};
    action.sa_handler = handler;
    action.sa_flags   = flags;
    sigemptyset(&action.sa_mask);
    if (sigaction(signum, &action, NULL) < 0) {
        error("Failed to register signal: %s", strerror(errno));
        return false;
    }
    return true;
}

/**
 * SIGALRM handler.
 * @param   signum      Signal number.
 **/
void sigalrm_handler(int signum) {
    // Fix errors w/ extern
    extern Scheduler PQShellScheduler;
    scheduler_wait(&PQShellScheduler);
    scheduler_next(&PQShellScheduler);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
