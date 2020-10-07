#!/usr/bin/env make

# FT2 # Makefile

	CXX = g++
	OPTIM += -g
	INCL += -I../
	INCL += -I../buffer
#	INCL = -I/tools/2000/X11R6/include
	dot_EXE=.elf

ifeq (1,0)
	INCL += `pkg-config --cflags freetype2`
	LIBS += `pkg-config --libs freetype2`
endif

ifeq (1,0)
	INCL += `pkg-config --cflags xrender`
	LIBS += `pkg-config --libs xrender`
endif

ifeq (1,1)
	INCL += `pkg-config --cflags $(PKGS)`
	LIBS += `pkg-config --libs $(PKGS)`
	PKGS += freetype2
	PKGS += xrender
endif

ifeq (0,1)
	FT2=/home/gps/YEAR/src_build_2020/FT2/freetype2
	INCL = -I $(FT2)/include
	OBJS += -L $(FT2)/objs/.libs
	test3_fon3 = /usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf
	OBJS += -lfreetype
endif


	DEFS += 
#	LIBS += -L/usr/X11R6/lib -lX11
#	LIBS += -lXrender
#	LIBS += -L/tools/2000/X11R6/lib -lX11

	S=../src
	S2=$S_WAX
	VPATH +=$S
	VPATH +=$(S2)
	INCL += -I$(S2)

#	include ../MK/Makefile_libs_apps
	include ../MK/MK_with_libs_apps

	HDRS += $(S2)/A_point_plus.h
	HDRS += $(S2)/X_Colours.h
	HDRS += $(S2)/X_Display.h
	HDRS += $(S2)/X_Draw.h
	HDRS += $(S2)/X_STUBS.h
	HDRS += $(S2)/X_Window.h

	OBJS += A_point_plus.o
	OBJS += A_Map_W.o
	OBJS += X_Window.o
	OBJS += X_Display.o
	OBJS += X_FontStr.o
	OBJS += X_ret_err.o
	OBJS += WAX_argv_decoder.o
	OBJS += test_X_two.o

#	OBJS += gdb_invoke.o
#	OBJS += str0.o
#	OBJS += str1.o
#	OBJS += blk1.o
#	OBJS += buffer1.o
#	OBJS += e_print.o

	OBJS += cpu_var_Name.o

	PROG1=test1
	PROG2=test3

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)

all: run_test3

.SUFFIXES: .cxx .c .o                                                           

# TODO move this back to libs_apps
# THEN use it's VARS + command line format
# .cxx.o:
# 	$(CXX) $(OPTIM) $(INCL) $(DEFS) -c $<                                  


$(OBJS): $(HDRS)

run_test1: $(EXEC1)
	./$(EXEC1)

run_test3: $(EXEC2)
	./$(EXEC2) $(test3_font)  P # "Sample Text"

$(EXEC1): $(OBJS) $(PROG1).o
	g++ $(PROG1).o $(OBJS) $(LIBS) -o $@

$(EXEC2): $(OBJS) $(PROG2).o
	g++ $(PROG2).o $(OBJS) $(LIBS) -o $@

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2)  test3 fa

$(EXEC2) $(EXEC1): ../MK/Makefile
