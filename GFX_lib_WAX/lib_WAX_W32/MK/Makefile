#!/usr/bin/env make

##	CXX = g++
	CXX=/usr/bin/i686-w64-mingw32-g++ -mwindows 
	OPTIM = -g
	INCLS = -I../
	INCLS = -I../buffer

#	project does not use any packages .ATM.
#	INCLS = `pkg-config --libs freetype2`
#	INCLS = `pkg-config --libs libs_Apps`

	dot_EXE=.exec	# this still works for WINE
	# dot_EXE=.exe# traditional different from UNIX

	# add any -D defines
	DEFS = 

#	# uncomment to see used # not WINE
#	INCLS = -I/tools/2000/X11R6/include
#	LINKS = -L/usr/X11R6/lib -lX11
#	LINKS = -L/tools/2000/X11R6/lib -lX11

# YES create the symb link
# which is the same as putting all DLL's on the PATH somewhere. dot
# the dir and the name will change with different distributions
# GCC auto requires it, no args added,

#	DLL_DIR=/usr/lib/gcc/i686-w64-mingw32/10-win32
	DLL_DIR=/usr/lib/gcc/i686-w64-mingw32/13-win32#/libgcc_s_dw2-1.dll
	DLL_FILE=libgcc_s_dw2-1.dll
	DLL_FULL=$(DLL_DIR)/$(DLL_FILE)

	S=../src
	VPATH +=$S
	VPATH +=$S/buffer


# off whilst build with W32
# 	include ../MK/Makefile_libs_apps # OK
	include ../MK/MK_with_libs_apps # OK - ish

#	HDRS += $S/A_point_plus.h
#	HDRS += $S/X_Colours.h
#	HDRS += $S/X_Display.h
#	HDRS += $S/X_Draw.h
#	HDRS += $S/X_STUBS.h
#	HDRS += $S/X_Window.h

#	# LOCAL objs

# EXEC already added #	OBJS += hello.o
#	OBJS += SPOUT_argv_decoder.o
#	OBJS += A_point_plus.o
#	OBJS += A_Map_W.o
#	OBJS += X_Window.o
#	OBJS += X_Display.o
#	OBJS += X_FontStr.o
#	OBJS += X_ret_err.o
#	OBJS += test_X_two.o

##	OBJS += gdb_invoke.o
##	OBJS += str0.o
##	OBJS += str1.o
##	OBJS += blk1.o
##	OBJS += buffer1.o
##	OBJS += e_print.o

#	PROG1=test1
 #	PROG1=hello
	PROG1=hello_1
	PROG2=test2

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)

all: run_test1

ifeq (0,1)
# include MK_SUFFIXES from MK_ext_lib
.SUFFIXES: .exe .exec .cxx .c .o                                                           
.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  
endif

# this adds OBJS twice via $^
# so dont use $^ use $(OBJS)
# see .o.exec in MK_SUFFIXES
# it should not hijack things but it does

$(OBJS): $(HDRS)

run_test1: $(EXEC1) 
	ls -l
	ls -ld $(EXEC1)
	# EXEC1 == $(EXEC1)
	# DIR_libs_apps == $(DIR_libs_apps)
	#
	wine ./$(EXEC1)

run_test2: $(EXEC2)
	./$(EXEC2) # "GEN_DOC GEN_PS GEN_SVG"

ifeq (0,1)
# only on WIN32 # this entire build is WIN32
$(EXEC1): gcc_dll
$(EXEC2): gcc_dll
endif

gcc_dll: $(DLL_FILE)
	# gcc_dll # rule used #

$(DLL_FILE): ../MK/Makefile
	#
	# The .exe needs $(DIR_FILE)
	#
	# ask_do does not work perfectly under less
	# but it does work
	#
	ask_do ln -sf $(DLL_FULL)

# without gcc.dll this is the message
#
# wine ./hello.exec
# 0024:err:module:import_dll Library libgcc_s_dw2-1.dll (which is needed by L"Z:\\home\\gps\\YEAR\\src\\PLAY\\GFX_lib_WAX\\lib_WAX_W32\\obj\\hello.exec") not found
# 0024:err:module:LdrInitializeThunk Importing dlls for L"Z:\\home\\gps\\YEAR\\src\\PLAY\\GFX_lib_WAX\\lib_WAX_W32\\obj\\hello.exec" failed, status c0000135
# make: *** [../MK/Makefile:101: run_test1] Error 53

ifeq (1,0)
# we no-longer link it - the MK_SUFFIXES .o.exec does
# It uses .. duuno ... vars ...
$(EXEC1): $(OBJS) $(PROG1).o
	g++ $(PROG1).o $(OBJS) $(LINKS) -o $@

$(EXEC2): $(OBJS) $(PROG2).o
	g++ $(PROG2).o $(OBJS) $(LINKS) -o $@
endif

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2) $(DLL_FILE) test2 fa
