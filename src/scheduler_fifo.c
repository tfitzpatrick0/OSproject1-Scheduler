/* scheduler_fifo.c: PQSH FIFO Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include <assert.h>

/**
 * Schedule next process using fifo policy:
 *
 *  As long as we have less running processes than our number of CPUS and
 *  there are waiting processes, then we should move a process from the
 *  waiting queue to the running queue.
 *
 * @param   s    Scheduler structure
 */
void scheduler_fifo(Scheduler *s) {
    // Check core usage and waiting size
    while(s->running.size < s->cores && s->waiting.size > 0){
        // Pop waiting
        Process * p = queue_pop(&s->waiting);
        // See if start worked
        if(!process_start(p)){
            // Push to finished
            queue_push(&s->finished, p);
        }
        else{
            // Push to running
            queue_push(&s->running, p);
        }
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
