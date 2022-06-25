/* queue.h: PQSH Queue */

#ifndef PQSH_QUEUE_H
#define PQSH_QUEUE_H

#include "pqsh/process.h"

#include <stdio.h>

/* Structure */

typedef struct Queue Queue;

struct Queue {
    Process *head;  /* Head of queue */
    Process *tail;  /* Tail of queue */
    size_t   size;  /* Size of queue */
};

/* Functions */

void        queue_push(Queue *q, Process *p);
Process *   queue_pop(Queue *q);
Process *   queue_remove(Queue *q, pid_t pid);
void        queue_dump(Queue *q, FILE *fs);

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
