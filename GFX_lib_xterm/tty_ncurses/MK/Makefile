#!/usr/bin/env make

	CXX = g++
	OPTIM += -g
	INCLS += -I./
	INCLS += -I../
#	INCLS += -I/tools/2000/X11R6/include

	DEFS = 
#	LINKS = -L/usr/X11R6/lib -lX11
	LINKS = -lncurses

	# S=../src
	S=..
	VPATH +=$S
	VPATH +=$S/src
	VPATH +=$S/src/PSG_Tree

	include ../MK/Makefile_libs_apps

	PROG1=test1
	PROG1_exec=$(PROG1).exec

	PROG2=test2
	PROG2_exec=$(PROG2).exec

	CLEAN1 += $(PROG1).o
	CLEAN2 += $(PROG2).o

	CLEAN1 += $(PROG1_exec)
	CLEAN2 += $(PROG2_exec)

	OBJS1 += tty_curses.o

	# order matters - maybe put into .a to mix it up

	CLEAN1 += $(OBJS1)
	CLEAN2 += $(OBJS2)

all: test1 test2

.SUFFIXES: .cxx .tab.c .tab.cc .cc .c .y .o

test1: $(PROG1_exec)
	./$(PROG1_exec)

.cc.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  

.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS1): $(HDRS)
$(OBJS2): $(HDRS)

#test1: $(PROG1_exec)
#	./$(PROG1_exec) gen

test2: $(PROG2_exec)
	# test2
	./$(PROG2_exec) < $S/test2.test

dgb: $(PROG1_exec)
	# debug
	gdb --args ./$(PROG1_exec) $(e1_lex).lex $(e1_yacc).y
#	gdb --args ./$(PROG2_exec) < $S/test2.test

$(PROG1_exec): $(PROG1).o $(OBJS1) 
	# g++ $< $(LINKS) -o $@
	g++ $(PROG1).o $(OBJS1) $(LINKS) -o $@

$(PROG2_exec): $(PROG2).o $(OBJS2) 
	# g++ $< $(LINKS) -o $@
	g++ $(PROG2).o $(OBJS2) $(LINKS) -o $@

clean: 
	rm -f $(CLEAN1) $(CLEAN2) 
	# rm -f $(CLEAN12) $(CLEAN23) 



