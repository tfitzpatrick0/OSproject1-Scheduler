# Configuration

CC		= gcc
LD		= gcc
AR		= ar
CFLAGS		= -g -std=gnu99 -Wall -Iinclude -fPIC #-DDEBUG=1
LDFLAGS		= -Llib
ARFLAGS		= rcs

# Variables

LIBRARY_HEADERS = $(wildcard include/pq/*.h)
LIBRARY_SOURCES = src/options.c src/process.c src/queue.c src/signal.c \
		  src/scheduler.c src/scheduler_fifo.c src/scheduler_rdrn.c \
		  src/timestamp.c
LIBRARY_OBJECTS	= $(LIBRARY_SOURCES:.c=.o)
STATIC_LIBRARY  = lib/libpqsh.a
PQSH_PROGRAM	= bin/pqsh

TEST_SOURCES    = $(wildcard tests/test_*.c)
TEST_OBJECTS	= $(TEST_SOURCES:.c=.o)
TEST_PROGRAMS   = $(subst tests,bin,$(basename $(TEST_OBJECTS)))
TEST_UNITS      = $(subst tests/,,$(basename $(shell ls tests/test_*.c)))
TEST_SCRIPTS	= $(subst tests/,,$(basename $(shell ls tests/test_*.sh)))

# Rules

all:	$(PQSH_PROGRAM)

%.o:	%.c $(LIBRARY_HEADERS)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

bin/%:	tests/%.o $(STATIC_LIBRARY)
	@echo "Linking $@"
	@$(LD) $(LDFLAGS) -o $@ $^

$(PQSH_PROGRAM):	src/pqsh.o $(STATIC_LIBRARY)
	@echo "Linking $@"
	@$(LD) $(LDFLAGS) -o $@ $^

$(STATIC_LIBRARY):	$(LIBRARY_OBJECTS)
	@echo "Linking $@"
	@$(AR) $(ARFLAGS) $@ $^

test:		$(TEST_PROGRAMS) $(PQSH_PROGRAM)
	@$(MAKE) -sk test-all

test-all:	test-units test-scripts

test-units:	$(TEST_UNITS)

test_%:		bin/test_%
	@printf "Testing %-20s ... " $@
	@failures=0
	@for i in $$(seq 0 $$($< 2>&1 | tail -n 1 | awk '{print $$1}')); do \
	    if ! $< $$i > tests/$@.log 2>&1; then \
	    	echo "FAILURE"; echo; cat tests/$@.log; echo; exit 1; \
	    fi \
	done
	@echo "SUCCESS"

test-scripts:	$(TEST_SCRIPTS)

test_%:		tests/test_%.sh
	@printf "\nTesting %-20s ...\n" $@
	@./tests/$@.sh

clean:
	@echo "Removing objects"
	@rm -f $(LIBRARY_OBJECTS) $(TEST_OBJECTS) src/*.o

	@echo "Removing static library"
	@rm -f $(STATIC_LIBRARY)

	@echo "Removing tests"
	@rm -f $(TEST_PROGRAMS)

	@echo "Removing pqsh"
	@rm -f $(PQSH_PROGRAM)

.PRECIOUS: %.o
