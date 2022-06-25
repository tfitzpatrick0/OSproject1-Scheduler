/* options.h: PQSH Options */

#ifndef PQSH_OPTIONS_H
#define PQSH_OPTIONS_H

#include "pqsh/scheduler.h"

#include <stdbool.h>

bool parse_command_line_options(int argc, char *argv[], Scheduler *s);

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
