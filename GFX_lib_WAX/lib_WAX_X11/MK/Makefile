#!/usr/bin/env make

# WAX
	## hmmm # new dependent on libs_apps

	CXX = g++
	OPTIM += -g
#	INCLS += -I../buffer
#	INCLS += -I/tools/2000/X11R6/include
#	INCLS += `pkg-config --libs freetype2`

	# nonstandard, but makes rules easier
	dot_EXE=.elf

	G_prefix=/home/gps/G/prefix
	PKG_CONFIG_dirs += $(G_prefix)/lib/pkgconfig/
	RPATHS += -Wl,--rpath=$(G_prefix)/lib
	LIBS += $(RPATHS)

	PKGS += freetype2
	PKGS += xft
	PKGS += fontconfig
	# src/IMG/
	PKGS += libpng
	PKGS += libturbojpeg

	# TODO docs for the VARS used
	INCLS += `pkg-config --cflags $(PKGS)`
	LINKS += `pkg-config --libs $(PKGS)`

ifeq (0,1)
	FT2=/home/gps/YEAR/src_build_2020/FT2/freetype2
	INCLS += -I $(FT2)/include
	LINKS += -L $(FT2)/objs/.libs
	LINKS += -lfreetype
endif

	test2_font = /usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf


	DEFS += 
	LINKS += -L/usr/X11R6/lib -lX11
#	LINKS = -L/tools/2000/X11R6/lib -lX11

	S=../src
	VPATH +=$S
	VPATH +=$(S)/IMG
#	VPATH +=$S/buffer
	INCLS += -I$(S)
	INCLS += -I$(S)/IMG

# OLD	include ../MK/Makefile_libs_apps
	include ../MK/MK_with_libs_apps_base

	# beware similarity with file in libs_apps

# rebuild everything after editing ANY makefile !!
	MAKES += ../MK/MK_with_libs_apps
	MAKES += ../MK/Makefile

# want RULE .o depends on .h
# this makes ALL .o's dependent on ALL .h's

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

all: run_test3

# all: run_test2

.SUFFIXES: .cxx .c .o                                                           

# .cxx.o:
# 	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS): $(HDRS)

gdb_test1: $(EXEC1)
	INGDB=1 gdb -tui --command ../.gdb_init_$(PROG1) --args ./$(EXEC1)

gdb_test2: $(EXEC2)
	INGDB=1 gdb -tui --args ./$(EXEC2) $(test2_font)  P 

run_test1: $(EXEC1)
	./$(EXEC1)

run_test2: $(EXEC2)
	./$(EXEC2) $(test2_font)  P # "Sample Text"

run_test3: $(EXEC3)
	./$(EXEC3) 

run_test4: $(EXEC4)
	./$(EXEC4) 

# pick up default .o.elf from MK_SUFFIXES from libs_apps
# but add .elf depends on ... same massive pool of $(OBJS)
# roll back into libs_apps as .so # LATER #

$(EXEC1): $(OBJS)
$(EXEC2): $(OBJS)
$(EXEC3): $(OBJS)
$(EXEC4): $(OBJS)

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2)  $(EXEC3)  test2 fa
