/* test_process.c: Test PQSH Process */

#include "pqsh/macros.h"
#include "pqsh/process.h"

#include <assert.h>

/* Constants */

const char *COMMAND = "sleep 10";

/* Test cases */

int test_00_process_create() {
    Process *p = process_create(COMMAND);
    assert(p);
    assert(streq(p->command, COMMAND));
    assert(p->pid == 0);
    free(p);
    return EXIT_SUCCESS;
}

int test_01_process_start() {
    Process *p = process_create(COMMAND);
    assert(p);
    assert(process_start(p));
    assert(p->pid > 0);

    char test[BUFSIZ];
    snprintf(test, BUFSIZ, "test -n \"$(ps ux | awk '$2 == %d {print $2}')\"", p->pid);
    assert(system(test) == 0);

    free(p);
    return EXIT_SUCCESS;
}

int test_02_process_pause() {
    Process *p = process_create(COMMAND);
    assert(p);
    assert(process_start(p));
    assert(p->pid > 0);
    assert(process_pause(p));
    
    char test[BUFSIZ];
    snprintf(test, BUFSIZ, "test \"$(ps ux | awk '$2 == %d {print substr($8, 0, 1)}')\" = T", p->pid);
    assert(system(test) == 0);
    free(p);
    return EXIT_SUCCESS;
}

int test_03_process_resume() {
    Process *p = process_create(COMMAND);
    assert(p);
    assert(process_start(p));
    assert(p->pid > 0);
    assert(process_pause(p));
    assert(process_resume(p));
    char test[BUFSIZ];
    snprintf(test, BUFSIZ, "test \"$(ps ux | awk '$2 == %d {print substr($8, 0, 1)}')\" = S", p->pid);
    assert(system(test) == 0);
    free(p);
    return EXIT_SUCCESS;
}

/* Main execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
	fprintf(stderr, "Where NUMBER is right of the following:\n");
	fprintf(stderr, "    0  Test process_create\n");
	fprintf(stderr, "    1  Test process_start\n");
	fprintf(stderr, "    2  Test process_pause\n");
	fprintf(stderr, "    3  Test process_resume\n");
	return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
	case 0:	status = test_00_process_create(); break;
	case 1:	status = test_01_process_start(); break;
	case 2:	status = test_02_process_pause(); break;
	case 3:	status = test_03_process_resume(); break;
	default:
	    fprintf(stderr, "Unknown NUMBER: %d\n", number);
	    break;
    }

    return status;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
