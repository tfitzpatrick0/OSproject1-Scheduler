/* pqsh.c: Process Queue Shell */

#include "pqsh/macros.h"
#include "pqsh/options.h"
#include "pqsh/scheduler.h"
#include "pqsh/signal.h"
#include <errno.h>
#include <string.h>
#include <sys/time.h>

/* Global Variables */
Scheduler PQShellScheduler = {
    .policy    = FIFO_POLICY,
    .cores     = 1,
    .timeout   = 250000,
};

/* Help Message */
void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}

/* Main Execution */
int main(int argc, char *argv[]) {
    Scheduler *s = &PQShellScheduler;

    // Parse command line options
    bool parseStatus = parse_command_line_options(argc, argv, s);
    if(!parseStatus){
        return EXIT_FAILURE;
    }
    // Register signal handlers
    bool sigHandlerStatus = signal_register(SIGALRM, 0, sigalrm_handler);
    if(!sigHandlerStatus){
        fprintf(stderr, "Handler failed to register");
        return EXIT_FAILURE;
    }
    // Start timer interrupt
    struct itimerval interval = {
        .it_interval = { .tv_sec = 0, .tv_usec = s->timeout },
        .it_value    = { .tv_sec = 0, .tv_usec = s->timeout },
    };
    if (setitimer(ITIMER_REAL, &interval, NULL) < 0) {
        fprintf(stderr, "Timer set failed");
        return EXIT_FAILURE;
    }
    // Process shell comands
    while (!feof(stdin)) {
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";
        char c[BUFSIZ] = "";

        printf("\nPQSH> ");

        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin));

        chomp(command);
        sscanf(command, "%s %[^,]", c, argument);

        // Handle the different commands
        if (streq(c, "help")) {
            help();
        }
        else if (streq(c, "exit") || streq(c, "quit")) {
            break;
        }
        else if(streq(c, "add")){
            scheduler_add(s, stdout, argument);
        }
        else if(streq(c, "status")){
            // Set a mask for the queue
            // Makes it easy to follow since we just call the function at the end rather than after each if
            int mask = WAITING || RUNNING || FINISHED;
            if(streq(argument, "waiting")){
                mask = WAITING;
            }
            else if(streq(argument, "running")){
                mask = RUNNING;
            }
            else if(streq(argument, "finished")){
                mask = FINISHED;
            }
            scheduler_status(s, stdout, mask);
        }
        else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        }
    }
    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
