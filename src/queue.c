/* queue.c: PQSH Queue */

#include "pqsh/macros.h"
#include "pqsh/queue.h"
#include <assert.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void        queue_push(Queue *q, Process *p) {
    // Check emptiness
    if(q->head == NULL){
        // Set head and tail to p
        q->head = p;
        q->tail = p;
    }
    // If not empty
    else {
        q->tail->next = p;
        q->tail = p;
    }
    // Update next to NULL and increment size
    p->next = NULL;
    q->size = q->size + 1;

}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process *   queue_pop(Queue *q) {
    // If empty then return null
    if(q->head == NULL){
      return NULL;
    }
    else {
        // Get head
        Process *p = q->head;
        // Set new head to next of curr head
        q->head = q->head->next;
        p->next = NULL;
        // If queue is empty now
        if(q->size == 0){
            // Tail is null
            q->tail = NULL;
        }
        // Update size
        q->size = q->size - 1;
        return p;
    }
}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process *   queue_remove(Queue *q, pid_t pid) {
    // Return null if empty
    if(q->head == NULL){
        return NULL;
    }
    // Get head
    Process * p = q->head;
    // Check pids
    if(pid == p->pid){
        // Head is now next and update size
        q->head = p->next;
        q->size = q->size - 1;
        return p;
    }

    // Current is the one after head
    Process * curr = q->head->next;
    // Loop until end of queue
    while (curr != NULL){
        // Check pids
        if(pid == curr->pid){
            p->next = curr->next;
            q->size = q->size - 1;
            // If we're at the tail
            if(curr == q->tail){
                // Set tail
                q->tail = p;
            }
            return curr;
        }
        p = curr;
        curr = curr->next;
    }

    return NULL;
}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void        queue_dump(Queue *q, FILE *fs) {
    // Get head
    Process *p = q->head;
    // Output
    fprintf(fs, "%6s %-30s %-13s %-13s %-13s\n",
                "PID", "COMMAND", "ARRIVAL", "START", "END");
    while(p != NULL){
        fprintf(fs, "%6ld %-30s %-13.2lf %-13.2lf %-13.2lf\n",
                (long)p->pid, p->command, p->arrival_time, p->start_time, p->end_time);
        // Go to next
        p = p->next;
    }

}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
