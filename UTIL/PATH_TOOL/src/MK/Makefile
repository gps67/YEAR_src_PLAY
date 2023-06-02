#!/usr/bin/env make

VPATH=../src

PROGNAME = PATH_TOOL

test: $(PROGNAME)
	../test1
	../test2
	../test3

all: test

CXX=g++ # linux
# CXX=CC # sxde # gmake
FLAGS_COMPILE += -g
FLAGS_COMPILE += -O2

$(PROGNAME).o: $(PROGNAME).cxx ../MK/Makefile
	$(CXX) $(FLAGS_COMPILE) -c -o $@ $<

$(PROGNAME): $(PROGNAME).o
	$(CXX) $(FLAGS_COMPILE) -o $@ $<
	# strip $@ # DONT
	cp -p $(PROGNAME) ..

clean:
	rm -f $(PROGNAME) $(PROGNAME).o


gdb: $(PROGNAME)
	INGDB=1 gdb -tui --command ../.gdbinit_$(PROGNAME) --args ./$(PROGNAME) -drop_no_dir add PATH /absent/dir
#	INGDB=1 gdb -tui --command ../.gdbinit_$(PROGNAME) --args ./$(PROGNAME) -drop_no_dir add PATH /absent/dir
