/* test_queue.c: Test PQSH Queue */

#include "pqsh/macros.h"
#include "pqsh/queue.h"

#include <assert.h>

/* Constants */

Process PROCESSES[] = {
    { "1", 1 },
    { "2", 2 },
    { "3", 3 },
    { "4", 4 },
    { "0", 0 },
};

/* Test cases */

int test_00_queue_push() {
    Queue q = {0};
    size_t i;

    for (i = 0; PROCESSES[i].pid; i++) {
    	queue_push(&q, &PROCESSES[i]);
    	assert(q.size == i + 1);
    }

    assert(q.size == i);

    i = 0;
    for (Process *p = q.head; p; p = p->next, i++) {
    	assert(streq(p->command, PROCESSES[i].command));
    	assert(p->pid == PROCESSES[i].pid);
    }

    return EXIT_SUCCESS;
}

int test_01_queue_pop() {
    Queue q = {0};
    size_t i, j;

    for (i = 0; PROCESSES[i].pid; i++) {
    	queue_push(&q, &PROCESSES[i]);
    }
    
    for (j = 0; PROCESSES[j].pid; j++) {
    	assert(q.head);
    	Process *p = queue_pop(&q);
    	assert(p);
    	assert(streq(p->command, PROCESSES[j].command));
    	assert(p->pid == PROCESSES[j].pid);
    	assert(q.size == i - j - 1);
    }
    assert(queue_pop(&q) == NULL);

    return EXIT_SUCCESS;
}

int test_02_queue_remove() {
    Queue q = {0};
    size_t i, j;

    for (i = 0; PROCESSES[i].pid; i++) {
    	queue_push(&q, &PROCESSES[i]);
    }
    
    for (j = 0; PROCESSES[j].pid; j++) {
    	assert(q.head);
    	Process *p = queue_remove(&q, j + 1);
    	assert(p);
    	assert(streq(p->command, PROCESSES[j].command));
    	assert(p->pid == PROCESSES[j].pid);
    	assert(q.size == i - j - 1);
    }
    assert(queue_remove(&q, 0) == NULL);

    return EXIT_SUCCESS;
}

/* Main execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
	fprintf(stderr, "Where NUMBER is right of the following:\n");
	fprintf(stderr, "    0. Test queue_push\n");
	fprintf(stderr, "    1. Test queue_pop\n");
	fprintf(stderr, "    2. Test queue_remove\n");
	return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
	case 0:	status = test_00_queue_push(); break;
	case 1:	status = test_01_queue_pop(); break;
	case 2:	status = test_02_queue_remove(); break;
	default:
	    fprintf(stderr, "Unknown NUMBER: %d\n", number);
	    break;
    }

    return status;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
