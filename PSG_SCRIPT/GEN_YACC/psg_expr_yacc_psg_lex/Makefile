#!/usr/bin/env make

	CXX = g++
	OPTIM += -g
	INCLS += -I./
	INCLS += -I../
	INCLS += -I../buffer
#	INCLS += -I/tools/2000/X11R6/include

	DEFS = 
#	LINKS = -L/usr/X11R6/lib -lX11

	# S=../src
	S=..
	VPATH +=$S
	VPATH +=$S/buffer
	# . is obj
	T=.

	# HDRS += $S/A_point_plus.h
	# HDRS += $S/X_Colours.h
	# HDRS += $S/X_Display.h
	# HDRS += $S/X_Draw.h
	# HDRS += $S/X_STUBS.h
	# HDRS += $S/X_Window.h

	# OBJS += A_point_plus.o
	# OBJS += A_Map_W.o
	# OBJS += X_Window.o
	# OBJS += X_Display.o

	# bunch of useful stuff
	OBJS += gdb_invoke.o
	OBJS += str0.o
	OBJS += str1.o
	OBJS += blk1.o
	OBJS += buffer1.o
	OBJS += e_print.o

	OBJS += e1.tab.o
	OBJS += e1_lex.o

	PROG2=test2
	PROG2_exec=$(PROG2).exec

all: test

.SUFFIXES: .cxx .tab.c .tab.cc .cc .c .y .o                                                           
$T/e1_lex.lex $T/e1.y: regen_libr.tcl regen_main.tcl regen_psg.tcl EXPRS.h
	$S/regen_main.tcl 

e1.tab.hh: e1.y 

# stupid .tab. part makes this less default
# -d outputs headers n defines
e1.tab.cc: e1.y 
	# bison -o $@ -L c++ $<
	bison -d -o $@ $<

e1_lex.cc: e1_lex.lex e1.tab.hh
	flex -o $@ $^

.cc.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  

.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS): $(HDRS)

test: $(PROG2_exec)
	./$(PROG2_exec) < $S/test1.test

$(PROG2_exec): $(PROG2).o $(OBJS) 
	# g++ $< $(LINKS) -o $@
	g++ $(PROG2).o $(OBJS) $(LINKS) -o $@

clean: 
	rm -f *.o $(PROG2_exec) test1 test2 fa e1.tab.c e1.tab.hh e1.tab.cc e1_lex.cc
	rm -f e1.y  e1_lex.lex
