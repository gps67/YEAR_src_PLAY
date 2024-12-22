#!/usr/bin/env make

	CXX = g++
	OPTIM = -g
	INCLS = -I../

	dot_EXE=.elf


	DEFS = 
#	LINKS = -L/usr/X11R6/lib -lX11

	S=../src
	VPATH +=$S

	include ../MK/Makefile_libs_apps

	PROG1=CET_read_file
	PROG2=test2

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)

#	OBJS += file_line_in.o

all: run_test1

.SUFFIXES: .cxx .c .o                                                           

.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS): $(HDRS)

run_test1: $(EXEC1)
	./$(EXEC1) | sort

run_test2: $(EXEC2)
	./$(EXEC2) $(test2_font)  P # "Sample Text"

$(EXEC1): $(OBJS) $(PROG1).o
	g++ $(PROG1).o $(OBJS) $(LINKS) -o $@

$(EXEC2): $(OBJS) $(PROG2).o
	g++ $(PROG2).o $(OBJS) $(LINKS) -o $@

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2)  test2 fa
