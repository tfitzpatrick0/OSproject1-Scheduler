#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include "pqsh/timestamp.h"
#include <errno.h>
#include <sys/wait.h>

/**
 * Add new command to waiting queue.
 * @param   s    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, FILE *fs, const char *command) {
    // Create process
    Process *p = process_create(command);
    if(!p){
        return;
    }
    // Push to waiting
    queue_push(&s->waiting, p);
}

/**
 * Display status of queues in Scheduler.
 * @param   s    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, FILE *fs, int queue) {
    fprintf(fs, "Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n"
    , s->running.size, s->waiting.size, s->finished.size, (s->total_turnaround_time / (double)s->finished.size), (s->total_response_time / (double)s->finished.size));
    // Check different possiblities for the queue
    if((RUNNING && queue) != 0){
        // If not empty then dump
        if(s->running.size){
            fprintf(fs, "\nRunning Queue:\n");
            queue_dump(&s->running, fs);
        }
    }
    if((WAITING && queue) != 0){
        // If not empty then dump
        if(s->waiting.size){
            fprintf(fs, "\nWaiting Queue:\n");
            queue_dump(&s->waiting, fs);
        }
    }
    if((FINISHED && queue) != 0){
        // If not empty then dump
        if(s->finished.size){
            fprintf(fs, "\nFinished Queue:\n");
            queue_dump(&s->finished, fs);
        }
    }
}

/**
 * Schedule next process using appropriate policy.
 * @param   s    Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {
    // Call appropriate scheduler
    if(s->policy == FIFO_POLICY){
        scheduler_fifo(s);
    }
    else if(s->policy == RDRN_POLICY){
        scheduler_rdrn(s);
    }
    else{
        fprintf(stderr, "Cannot find policy");
    }
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s    Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {
    /* Wait for any children without blocking:
     *  - Remove process from queues.
     *  - Update Process metrics.
     *  - Update Scheduler metrics.
     */
    pid_t pid;
    // Wait w/o blocking
    while((pid = waitpid(-1, NULL, WNOHANG)) > 0){
        Process *found = queue_remove(&s->running, pid);
        if(!found){
            found = queue_remove(&s->waiting, pid);
            if(!found){
                continue;
            }

        }
        // Set and update times
        found->end_time = timestamp();
        queue_push(&s->finished, found);
        s->total_turnaround_time += (double)(found->end_time - found->arrival_time);
        s->total_response_time += (double)(found->start_time - found->arrival_time);
    }


}


/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
