#!/usr/bin/env make

# FT2 # Makefile
	# the problem with "test2" is that test2.cxx exists in WAX
	# so the default rule g++ uses all the right flags
	# but then the default rule cc test2.o -o test2 produces junk
	# same as THIS makefile has a rule that depends on $(OBJS)
	# so links them all in
	# FIX edit run_test2 instead

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

	PKGS += libpng
	PKGS += freetype2
	PKGS += xft
	PKGS += fontconfig

	G_prefix=/home/gps/G/prefix
	PKG_CONFIG_dirs += $(G_prefix)/lib/pkgconfig/
	RPATHS += -Wl,--rpath=$(G_prefix)/lib
	LIBS += $(RPATHS)


	PKGS += libturbojpeg

	# these are supposed to be part of the build system
	# TODO docs for the VARS used
	INCLS += `pkg-config --cflags $(PKGS)`
	LINKS += `pkg-config --libs $(PKGS)`

	DEFS += 
	LIBS += -L/usr/X11R6/lib -lX11
#	LIBS += -L/tools/2000/X11R6/lib -lX11

	S=../src
#	# borrow some source from WAX # duplicate compile but not fork
#	S2=../../../GFX_lib_WAX/lib_WAX_X11/src
	YEAR_src_PLAY=../../..
	S2=$(YEAR_src_PLAY)/GFX_lib_WAX/lib_WAX_X11/src

	VPATH +=$S
	VPATH +=$(S2)
	INCL += -I$(S2)


#	include ../MK/Makefile_libs_apps
	include ../MK/MK_with_libs_apps

	MAKES += ../MK/MK_with_libs_apps
	MAKES += ../MK/Makefile

ifeq (1,1)
	# WAX headers
	HDRS += $(S2)/A_point_plus.h
	HDRS += $(S2)/X_Colours.h
	HDRS += $(S2)/X_Display.h
	HDRS += $(S2)/X_Draw.h
	HDRS += $(S2)/X_STUBS.h
	HDRS += $(S2)/X_Window.h
	HDRS += $(S2)/X_Pixmap.h

	# WAX objs
	OBJS += A_point_plus.o
	OBJS += A_Map_W.o
	OBJS += X_Window.o
	OBJS += X_Pixmap.o
	OBJS += X_Display.o
	OBJS += X_FontStr.o
	OBJS += X_ret_err.o
	OBJS += WAX_argv_decoder.o

endif
ifeq (1,0)
	OBJS += test_X_two.o
	OBJS += XFT.o
	OBJS += A_matrix_2x2.o

#	# libs_apps alternatives
#	OBJS += gdb_invoke.o
#	OBJS += str0.o
#	OBJS += str1.o
#	OBJS += blk1.o
#	OBJS += buffer1.o
#	OBJS += e_print.o

	OBJS += ft2.o
endif

	OBJS += png_one.o
	OBJS += u32_RGBA.o
	OBJS += tbl_enum.o
	OBJS += TJ_blk.o
	OBJS += TJ_scale_factor.o
	OBJS += TJ_values.o
	OBJS += TJ_instance.o
	OBJS += TJ_loader.o
	OBJS += TJ_saver.o
	OBJS += IMG_loader.o


	PROG1=test1_libpng
	PROG2=test2_libjpeg

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)

all: run_test1 run_test2


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

test1 run_test2: $(EXEC2)
	./$(EXEC2) 

$(EXEC1): $(MAKES)

$(EXEC2): $(MAKES)

$(EXEC1): $(OBJS) $(PROG1).o
	g++ $(PROG1).o $(OBJS) $(LIBS) -o $@

$(EXEC2): $(OBJS) $(PROG2).o
	# VPATH is $(VPATH)
	# PKGS  is $(PKGS)
	# INCL  is $(INCL)
	# LIBS  is $(LIBS)
	g++ $(PROG2).o $(OBJS) $(LIBS) -o $@

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2)  test2 fa
