#!/usr/bin/env make

	CXX = g++
	OPTIM = -g
	INCLS = -I../
	INCLS = -I../buffer
#	INCLS = -I/tools/2000/X11R6/include
#	INCLS = `pkg-config --libs freetype2`

ifeq (0,1)
	FT2=/home/gps/YEAR/src_build_2020/FT2/freetype2
	INCLS = -I $(FT2)/include
	OBJS += -L $(FT2)/objs/.libs
	test2_font = /usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf
	OBJS += -lfreetype
endif


	DEFS = 
	LINKS = -L/usr/X11R6/lib -lX11
#	LINKS = -L/tools/2000/X11R6/lib -lX11

	S=../src
	VPATH +=$S
	VPATH +=$S/buffer

	include ../MK/Makefile_libs_apps

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

#	OBJS += gdb_invoke.o
#	OBJS += str0.o
#	OBJS += str1.o
#	OBJS += blk1.o
#	OBJS += buffer1.o
#	OBJS += e_print.o

	PROG1=test1
	PROG2=test2

all: run_test2

.SUFFIXES: .cxx .c .o                                                           

.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS): $(HDRS)

run_test1: $(PROG1)
	./$(PROG1)

run_test2: $(PROG2)
	./$(PROG2) $(test2_font)  P # "Sample Text"

$(PROG1): $(OBJS) $(PROG1).o
	g++ $(PROG1).o $(OBJS) $(LINKS) -o $(PROG1)

$(PROG2): $(OBJS) $(PROG2).o
	g++ $(PROG2).o $(OBJS) $(LINKS) -o $(PROG2)

clean: 
	rm -f *.o test1 test2 fa
