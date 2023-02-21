#!/usr/bin/env make

# WAX
	## hmmm # now dependent on libs_apps

	CXX = g++
	OPTIM += -g

	# nonstandard, but makes rules easier
	dot_EXE=.elf

#	# UNUSED OPTIONS # maybe used via MK_with
#	G_prefix=/home/gps/G/prefix
#	LIST_PKG_CONFIG_PATH += $(G_prefix)/lib/pkgconfig/
#	LIST_RPATH += $(G_prefix)/lib

	PKGS += freetype2
	PKGS += xft
	PKGS += fontconfig
	PKGS += libpng
	PKGS += libturbojpeg
	PKGS += libs_apps_base1

	LINKS += -rdynamic # export all symbols - backtrace works!

#	# TODO docs for the VARS used
#	INCLS += `pkg-config --cflags $(PKGS)`
#	LINKS += `pkg-config --libs $(PKGS)`
#	LINKS += $(RPATHS)

	test2_font = /usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf


	DEFS += 
	LINKS += -L/usr/X11R6/lib -lX11
#	LINKS = -L/tools/2000/X11R6/lib -lX11

	# S=../src
	S=/home/gps/YEAR/src/GITS/YEAR_src_PLAY/GFX_lib_WAX/lib_WAX_X11/src
	VPATH +=$S
	VPATH +=$(S)/IMG

	INCLS += -I$(S)
	INCLS += -I$(S)/IMG

	# to build with libs_apps base1 create this symb-link and include it
	# a symb link to libs_apps 
	include ../MK/MK_with_libs_apps

# rebuild everything after editing ANY makefile !!

	MAKES += ../MK/MK_with_libs_apps
	MAKES += ../MK/Makefile

# want RULE .o depends on .h
# this makes ALL .o's dependent on ALL .h's

	# although this is IMG it uses a lot of WAX
	# that will become PKGS += libs_apps_WAX # maybe

	HDRS += $S/A_point_plus.h
	HDRS += $S/X_Colours.h
	HDRS += $S/X_Display.h
	HDRS += $S/X_Draw.h
	HDRS += $S/X_STUBS.h
	HDRS += $S/X_Window.h
	HDRS += $S/XFT.h

	# after any HRDS edited, rebuild OBJS in this order

	OBJS += test_X_two.o
	OBJS += XFT.o

	OBJS += WAX_argv_decoder.o

	OBJS += A_point_plus.o
	OBJS += A_Map_W.o
	OBJS += X_Window.o
	OBJS += X_Display.o
	OBJS += X_FontStr.o
	OBJS += X_ret_err.o
	OBJS += A_matrix_2x2.o
	OBJS += X_Drawable_Surface.o
	OBJS += X_GC.o
	OBJS += X_Pixmap.o

	# OBJS_IMG

	OBJS += png_one.o
	OBJS += u32_RGBA.o
	OBJS += tbl_enum.o
	OBJS += TJ_blk.o
	OBJS += TJ_scale_factor.o
	OBJS += TJ_values.o
	OBJS += TJ_instance.o
	OBJS += TJ_loader.o
	OBJS += TJ_saver.o
#	OBJS += IMG_loader.o
	# here for now
	OBJS += X_Image.o

#	we nolonger build a few files, now with -I libs_apps/include -lbase
#	this was ALL that we need from base1 though
#	OBJS += gdb_invoke.o
#	OBJS += str0.o
#	OBJS += str1.o
#	OBJS += blk1.o
#	OBJS += buffer1.o
#	OBJS += e_print.o

	PROG1=test1_nested_pair
	PROG2=test2_xft_diagonal
	PROG3=test_libpng
	PROG4=test_libjpeg

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)
	EXEC3=$(PROG3)$(dot_EXE)
	EXEC4=$(PROG4)$(dot_EXE)
#	gdb=gdb
	gdb=/tools/x86_64/ccs/bin/gdb

all: run_test3

# all: run_test2

.SUFFIXES: .cxx .c .o                                                           

# .cxx.o:
# 	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS): $(HDRS)

gdb_test1: $(EXEC1)
	INGDB=1 $(gdb) -tui --command ../.gdb_init_$(PROG1) --args ./$(EXEC1)

gdb_test2: $(EXEC2)
	INGDB=1 $(gdb) -tui --command ../.gdb_init_$(PROG2) --args ./$(EXEC2) $(test2_font)  P 

gdb_test3: $(EXEC3)
	INGDB=1 $(gdb) -tui --command ../.gdb_init_$(PROG3) --args ./$(EXEC3)

run_test1: $(EXEC1)
	./$(EXEC1)

run_test2: $(EXEC2)
	./$(EXEC2) $(test2_font)  P # "Sample Text"

run_test3: $(EXEC3)
	./$(EXEC3) 

run_test4: $(EXEC4)
	./$(EXEC4) 

lots: $(EXEC1) $(EXEC2) $(EXEC2) $(EXEC2) 
	# make lots

# pick up default .o.elf from MK_SUFFIXES from libs_apps
# but add .elf depends on ... same massive pool of $(OBJS)
# roll back into libs_apps as .so # LATER #

$(EXEC1): $(OBJS)
$(EXEC2): $(OBJS)
$(EXEC3): $(OBJS)
$(EXEC4): $(OBJS)

relink: 
	rm -f $(EXEC1)  $(EXEC2)  $(EXEC3)  test2 fa

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2)  $(EXEC3)  test2 fa
