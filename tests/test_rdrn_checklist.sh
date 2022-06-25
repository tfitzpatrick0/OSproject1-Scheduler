#!/bin/sh

LOGFILE=tests/test_rdrn_checklist.log

# Functions

pqsh_test_commands() {
    echo add bin/worksim 3
    sleep 1
    echo add bin/worksim 2
    sleep 1 
    echo add bin/worksim 1
    sleep 6
    echo "status"
}

pqsh_check_output() {
    python3 <<EOF
import re
import sys

lines = open("$LOGFILE").readlines()
try:
    running, waiting, finished, turnaround, response = \
	[float(line.split('=')[-1]) for line in lines[-9].split(',')]
except ValueError:
    sys.exit(1)

if running != 0 or waiting != 0 or finished != 3:
    sys.exit(2)

if not (4.75 <= turnaround <= 5.75):
    sys.exit(3)

if not (0.00 <= response <= 0.75):
    sys.exit(4)
EOF
}

# Main Execution

if [ ! -x bin/pqsh ]; then
    echo "ERROR: Please build bin/pqsh"
    exit 1
fi

echo -n "  Running PQSH commands      ... "
if pqsh_test_commands | valgrind --leak-check=full bin/pqsh -p rdrn > $LOGFILE 2> $LOGFILE.valgrind; then
    echo "SUCCESS"
else
    echo "FAILURE"
    exit 2
fi

echo -n "  Verifying PQSH output      ... "
if pqsh_check_output; then
    echo "SUCCESS"
else
    echo "FAILURE"
    exit 3
fi

echo -n "  Verifying PQSH memory      ... "
if [ $(awk '/ERROR SUMMARY:/ {print $4}' $LOGFILE.valgrind) -eq 0 ]; then
    echo "SUCCESS"
else
    echo "FAILURE"
    exit 4
fi
