/* scheduler_rdrn.c: PQSH Round Robin Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include <assert.h>

/**
 * Schedule next process using round robin policy:
 *
 *  1. If there no waiting processes, then do nothing.
 *
 *  2. Move one process from front of running queue and place in back of
 *  waiting queue.
 *
 *  3. Move one process from front of waiting queue and place in back of
 *  running queue.
 *
 * @param   s    Scheduler structure
 **/
void scheduler_rdrn(Scheduler *s) {
    // If empty then just return
    if(s->waiting.size == 0){
        return;
    }

    Process * p;
    if(s->running.size == s->cores){
        // Pop running
        p = queue_pop(&s->running);
        // Check if pause worked
        if(!process_pause(p)){
            // Push running
            queue_push(&s->running, p);
        }
        else{
            // Push waiting
            queue_push(&s->waiting, p);
        }

    }
    // Check waiting isn't empty and compare running size w/ cores
    while(s->waiting.size != 0 && s->running.size < s->cores){
        p = queue_pop(&s->waiting);
        // Check pid to see if it has started
        if(p->pid != 0){
          // Resume and check if it worked
          if(!process_resume(p)){
              fprintf(stderr, "Couldn't resume process");
              queue_push(&s->waiting, p);
              continue;
          }
        }
        else{
            // Start and see if it starts properly
            if(!process_start(p)){
                fprintf(stderr, "Waiting process couldn't be started");
                // Push to finished
                queue_push(&s->finished, p);
                continue;
            }
        }
        // Push to running queue
        queue_push(&s->running, p);
    }


}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
