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

# OTHER OPTION: 
#	OBJS1 += $(PROG1).o
#	OBJS2 += $(PROG2).o
#	OBJS2 += $(OBJ1)
#	OBJS2 = & OBJS1 # HMMM syntax of CT implement B within C bind_var
#	DIALECT EA_EXPR OBJS_LIST_1 = POOL of filename.o or OTHER
#	DIALECT EA_EXPR OBJS_LIST_2 = POOL of filename.o or OTHER
# 	LEXICON EA_EXPR ...
#	Makefile
#	LATER CLEAN += OBJS # so dont add here, add there ...
	CLEAN1 += $(PROG1).o
	CLEAN2 += $(PROG2).o

	CLEAN1 += $(PROG1_exec)
	CLEAN2 += $(PROG2_exec)

	OBJS1 += tty_curses.o
	OBJS1 += tty_curses_CSR.o
	OBJS1 += tty_curses_test_1.o

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
	# rm -f $(CLEAN12) $(CLEAN23) 
	rm -f $(CLEAN1) $(CLEAN2) 


# I need to check those different spellings in the comments
# But easier to wait until EXPR_Makefile brings its LEXICON $< means ...

# PROG1 generated FILE2
# FILE2 generated RESULT3
# now tidy up, each activity by its own list, generated in the way from 1 to 2
# CLEAN12 
# now tidy up, each activity by its own list, generated in the way from 2 to 3
# CLEAN23 
# Maybe RESULT is still there ?
