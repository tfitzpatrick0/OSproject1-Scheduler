/* process.h: PQSH Process */

#ifndef PQSH_PROCESS_H
#define PQSH_PROCESS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Constants */

#define MAX_ARGUMENTS   1024

/* Structure */

typedef struct Process      Process;

struct Process {
    char    command[BUFSIZ];    /* Command to execute */
    pid_t   pid;                /* Process identifier (0 == invalid) */
    double  arrival_time;       /* Process arrival time (is placed into waiting queue) */
    double  start_time;         /* Process start time (is first placed into running queue) */
    double  end_time;           /* Process end time (is placed into finished queue) */

    Process *next;              /* Pointer to next process */
};

/* Functions */

Process *   process_create(const char *command);
bool        process_start(Process *p);
bool        process_pause(Process *p);
bool        process_resume(Process *p);

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
