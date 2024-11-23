#!/usr/bin/env make

# FT2 # Makefile

	CXX = g++
	OPTIM += -g
	INCL += -I../
#	dont confuse MK_with_libs_apps
#	INCL += -I../buffer

#	unusual X11 build 
#	INCL = -I/tools/2000/X11R6/include
	dot_EXE=.elf


#	KLUDGE # this is not how to PKG see X11 or elsewhere
	INCL += $(INCLS)
	LIBS += $(LINKS)

	PKGS += freetype2
	PKGS += xft
	PKGS += fontconfig
	PKGS += libs_apps_base1


ifeq (1,1)
	# these are supposed to be part of the build system
	# TODO docs for the VARS used
	INCLS += `pkg-config --cflags $(PKGS)`
	LINKS += `pkg-config --libs $(PKGS)`
endif

ifeq (1,0)
	INCL += `pkg-config --cflags freetype2`
	LIBS += `pkg-config --libs freetype2`
endif

ifeq (0,1)
	FT2=/home/gps/YEAR/src_build_2020/FT2/freetype2
	INCL = -I $(FT2)/include
	OBJS += -L $(FT2)/objs/.libs
	test2_font = /usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf
	OBJS += -lfreetype
endif


	DEFS += 
	LIBS += -L/usr/X11R6/lib -lX11
#	LIBS += -L/tools/2000/X11R6/lib -lX11

	S=../src
#	# borrow some source from WAX # duplicate compile but not fork
	YEAR_src_PLAY=../../..
	S2=$(YEAR_src_PLAY)/GFX_lib_WAX/lib_WAX_X11/src
	S3=$(YEAR_src_PLAY)/GFX_lib_WAX/lib_WAX_X11/src/APPS

# /home/gps/YEAR/src/PLAY/GFX_lib_WAX/lib_WAX_X11/src/APPS

# /home/gps/G_laptop/G_2T/G_HOME_2021/tmp_tmp/t_git/
#	YEAR_src_PLAY/GFX_lib_WAX/lib_WAX_X11/src/APPS/test_X_one.h

	VPATH +=$S
	VPATH +=$(S2)
	VPATH +=$(S3)
	INCL += -I$(S2)
	INCL += -I$(S3)
	# TODO # FIX # above fails below works # so $(INCL)
	LIST_C_INCLUDE_PATH += $(S2)
	LIST_C_INCLUDE_PATH += $(S3)
	LIST_CPLUS_INCLUDE_PATH += $(S2)
	LIST_CPLUS_INCLUDE_PATH += $(S3)


#	include ../MK/Makefile_libs_apps
	include ../MK/MK_with_libs_apps

	MAKES += ../MK/MK_with_libs_apps
	MAKES += ../MK/Makefile

	# WAX headers
	HDRS += $(S2)/A_point_plus.h
	HDRS += $(S2)/X_Colours.h
	HDRS += $(S2)/X_Display.h
	HDRS += $(S2)/X_Draw.h
	HDRS += $(S2)/X_STUBS.h
	HDRS += $(S2)/X_Window.h

	# WAX objs
	OBJS += A_point_plus.o
	OBJS += A_Map_W.o
	OBJS += X_Window.o
	OBJS += X_Display.o
	OBJS += X_FontStr.o
	OBJS += X_ret_err.o
	OBJS += WAX_argv_decoder.o
	OBJS += test_X_two.o
	OBJS += XFT.o
	OBJS += A_matrix_2x2.o

	OBJS += X_Drawable_Surface.o

#	# libs_apps alternatives
#	OBJS += gdb_invoke.o
#	OBJS += str0.o
#	OBJS += str1.o
#	OBJS += blk1.o
#	OBJS += buffer1.o
#	OBJS += e_print.o

	OBJS += ft2.o

	PROG1=test1
	PROG2=test2

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)

all: run_test2

ifeq (1,0)
.SUFFIXES: .cxx .c .o                                                           

# this is in MK_SUFFIXES
# I need to document my choice of VARS
.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCL) $(DEFS) -c $<                                  
endif


$(OBJS): $(HDRS)

run_test1: $(EXEC1)
	./$(EXEC1)

run_test2: $(EXEC2)
	./$(EXEC2) $(test2_font)  P # "Sample Text"

gdb_test2: $(EXEC2)
	gdb -tui --command .gdbinit_$(PROG2)  ./$(EXEC2) $(test2_font)  P # "Sample Text"

$(EXEC1): $(MAKES)

$(EXEC2): $(MAKES)

$(EXEC1): $(OBJS) $(PROG1).o
	# EXEC1 #
	# VPATH is $(VPATH)
	# PKGS  is $(PKGS)
	# INCL  is $(INCL)
	# LIBS  is $(LIBS)
	g++ $(PROG1).o $(OBJS) $(LIBS) -o $@

$(EXEC2): $(OBJS) $(PROG2).o
	# EXEC2 #
	# VPATH is $(VPATH)
	# PKGS  is $(PKGS)
	# INCL  is $(INCL)
	# LIBS  is $(LIBS)
	g++ $(PROG2).o $(OBJS) $(LIBS) -o $@

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2)  test2 fa
