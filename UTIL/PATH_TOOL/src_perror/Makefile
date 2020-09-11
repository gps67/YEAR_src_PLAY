#!/usr/bin/env make

PROGNAME = perror
EXECNAME = $(PROGNAME).exec

test: $(EXECNAME)
	./test1
	./test2
	./test3

FLAGS_COMPILE += -g
FLAGS_COMPILE += -O2
# NO # FLAGS_COMPILE += -nostdlib # exceptions need a basic C++ lib

$(PROGNAME).o: $(PROGNAME).c Makefile
	gcc $(FLAGS_COMPILE) -c -o $@ $<

$(EXECNAME): $(PROGNAME).o
	gcc $(FLAGS_COMPILE) -o $@ $<
	strip $@
	cp -p $(EXECNAME) ..
	cp -p $(EXECNAME) ../../bin/

../$(PROGNAME)_static: $(PROGNAME).o
	gcc $(FLAGS_COMPILE) -static -o $@ $<
	strip $@

clean:
	rm -f $(EXECNAME) $(PROGNAME).o

