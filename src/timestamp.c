/* timestamp.c: PQSH Timestamp */

#include "pqsh/timestamp.h"

#include <time.h>
#include <sys/time.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes gettimeofday for precision (accounting for seconds and
 * microseconds) and falls back to time (only account for seconds) if that
 * fails.
 *
 * @return  Double representing the current time.
 **/
double timestamp() {
    // Struct for time
    struct timeval current_time;
    // Get time and see if it worked
    int status = gettimeofday(&current_time, NULL);
    // Return certain time based on the status
    if(status == 0){
      return (current_time.tv_sec + 0.0000001 * current_time.tv_usec);
    }
    return time(NULL);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
