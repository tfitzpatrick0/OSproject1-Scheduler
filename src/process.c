/* process.c: PQSH Process */

#include "pqsh/macros.h"
#include "pqsh/process.h"
#include "pqsh/timestamp.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process * process_create(const char *command) {
    // Allocate for process
    Process * p = (Process *)calloc(1, sizeof(Process));
    if (!p){
        return NULL;
    }
    // Set command and arrival time
    strcpy(p->command, command);
    p->arrival_time = timestamp();
    return p;
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {
    // Fork and set pid
    pid_t pid = fork();
    p->pid = pid;

    // Fails if pid less than zero
    if(pid < 0){
        return false;
    }
    // Child
    if (pid == 0){
        char *argv[MAX_ARGUMENTS] = {0};
        char command[BUFSIZ];
        // Set command
        sprintf(command, p->command);
        int i = 0;

        for (char *token = strtok(command, " "); token; token = strtok(NULL, " ")) {
            argv[i++] = token;
        }
        // Exec
        execvp(argv[0], argv);

        exit(EXIT_FAILURE);
    }
    else{
        // Parent updates start time
        p->start_time = timestamp();
    }

    return true;
}

/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {
    // Check SIGSTOP response
    return (kill(p->pid, SIGSTOP) > -1);
}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_resume(Process *p) {
    // Check SIGCONT response
    return (kill(p->pid, SIGCONT) > -1);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
