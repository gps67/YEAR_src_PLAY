#!/usr/bin/env make

	CXX = g++
	OPTIM = -g
	INCLS = -I../
	INCLS = -I../buffer
#	INCLS = -I/tools/2000/X11R6/include

	DEFS = 
	LINKS = -L/usr/X11R6/lib -lX11
#	LINKS = -L/tools/2000/X11R6/lib -lX11

	S=../src
	VPATH +=$S
	VPATH +=$S/buffer

	HDRS += $S/A_point_plus.h
	HDRS += $S/X_Colours.h
	HDRS += $S/X_Display.h
	HDRS += $S/X_Draw.h
	HDRS += $S/X_STUBS.h
	HDRS += $S/X_Window.h

	OBJS += A_point_plus.o
	OBJS += A_Map_W.o
	OBJS += X_Window.o
	OBJS += X_Display.o

	OBJS += gdb_invoke.o
	OBJS += str0.o
	OBJS += str1.o
	OBJS += blk1.o
	OBJS += buffer1.o
	OBJS += e_print.o

	PROG2=test2

all: test

.SUFFIXES: .cxx .c .o                                                           

.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS): $(HDRS)

test: $(PROG2)
	./$(PROG2)

$(PROG2): $(OBJS) $(PROG2).o
	g++ $(PROG2).o $(OBJS) $(LINKS) -o $(PROG2)

clean: 
	rm -f *.o test1 test2 fa
