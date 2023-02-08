#!/usr/bin/make -f
# a stand alone project that uses libs_apps
# uses the 2002 Makefile layout (ongoing++)
# LIST_XXX += somedir

# names of the programs (but not with .exec which is in $(EXE_1))

	PROG1 =TTY# NOSP # run ARGS in a TTY
	PROG2 =TTY_EMUL# NOSP # run ARGS in a TTY with XTERM EMUL # TODO

#	TGT=my_name_for_architecture_to_trigger -mARCH or other
#	probably a bit late by now, as GXX might be a cross-compile
#	needed to find .../libs_apps/lib_pkgconfig/LGX/
#	needed to find .../libs_apps/build_LGX_UNAME/
#
# done in MK_with_libs_apps # 
#
#	TGT=WIN32
#	TGT=LGX

# TODO flags --tee logfilename
# TODO flags --tee_a logfilename

# we are in $T/obj/.

	T=..

# hardcoded DIR where libs_apps is

	# You will probably move this
	# it is actually a dir called YEAR_src_PLAY from github gps67

	YEAR_src_PLAY=$(HOME)/YEAR/src/PLAY

	# subdir in that project where makefiles are
	# which will also need editing (soz)
	MK_ext_lib=$(YEAR_src_PLAY)/libs_apps/MK_ext_lib

	# see include $(MK_ext_lib)/MK_with_libs_apps # BELOW #

	# the alternative is a symb link in this dir
	# that link does exist (slightly renamed)
	# to confuse you
	# and to help view it
	# either way thats 3 things to change
	# you need to find it
	# it needs to find it
	# it find libs_apps_base1 which needs to find thae dir

# generic GCC world # customised to this project

	CXX = g++
	OPTIM += -g 

# libs_apps MK_LISTS converts this to colon separated CPLUS_INCLUDE_PATH

	LIST_CPLUS_INCLUDE_PATH += $T/src/

# I add this macro, for anything like -DNAME or -DNAME=VALUE

	DEFS = 

# GCC searches for source files along VPATH: MK_LISTS converts SP to :

	LIST_VPATH +=$T
	LIST_VPATH +=$T/src
#	LIST_VPATH +=$T/src/some_sub_dir
#	YACC generates files in .
	LIST_VPATH+=:$T/objs
#	LIST_VPATH+=:.

# ld.so uses a runtime search path for libXXX.sh 
# MK_LISTS converts LIST_RPATH to -Wl,--rpath=$(dir))
# that tells GCC to pass --rpath=dir to LD # man ld
# system wide config is is a different option 

# I use this for manually configured things into testing prefix dir

#	# libs_apps # brings this
# EG	DIR_G_prefix =/home/gps/G/prefix
# EG	LIST_RPATH += $(DIR_G_prefix)/SM/lib

# MK_LISTS converts LIST_PKG_CONFIG_PATH to : PKG_CONFIG_PATH
# rather uselessly the .../lib/pkgconfig is required
# this is used by pkg-config to find PKGXXX.pc

#	# libs_apps # brings this
#        LIST_PKG_CONFIG_PATH += $(DIR_G_prefix)/lib/pkgconfig/
#        LIST_PKG_CONFIG_PATH += $(DIR_G_prefix)/SM/lib/pkgconfig/
#        LIST_PKG_CONFIG_PATH += /test/dir/lib/pkgconfig/

# include MK_with_libs_apps ... MK_LISTS ... MK_SUFFIXES ...

	include $(MK_ext_lib)/MK_with_libs_apps # TODO put back there

#	# which includes these
#	include $(MK_ext_lib)/MK_SUFFIXES
#	include $(MK_ext_lib)/MK_LISTS_

# PKGS # build with flags dirs libs from pkg-config

	PKGS += libs_apps_base1
	PKGS += libs_apps_base2
	PKGS += libs_apps_uses_ssl
	PKGS += libs_apps_mozjs
	# mozjs is included by others anyway

# .exec files are used to make new makefiles easier ? and WIN32 too

	EXE_1 =$(PROG1).exec# NOSP
	EXE_2 =$(PROG2).exec# NOSP

# one way of making all targets

	EXELIST += $(EXE_1)
	EXELIST += $(EXE_2)

#	# no need to add main.o it will be depended on anyway
#	OBJS += $(PROG1).o

#	# do add the long list of .o files that go into PROG1 and PROG2
#	# only the main is different, so some waste code in PROG3

#	OBJS += some_thing_useful.o
#	OBJS += some_thing_used.o
#	OBJS += some_thing_used.o
#	OBJS += some_thing_used.o

#	OBJS_PROG3 += needed.o for PROG3 # unused in this simple model
#	OBJS_PROG4 += needed.o for PROG4 # if also needed there

# TEST for last include last #
#	include $(MK_ext_lib)/MK_with_libs_apps # TODO put back there
	#

#
# DEFAULT COMES FIRST
#

default: show_vars P default_test

defaults2: show_vars

# show_vars #
#	# uses a neat Makefile trick
#	# comments are echoed to stdout
#	# with variables expanded

show_vars:
	# NOWT TO DO HERE
	#
	# VPATH #
	#   $(VPATH)
	#
	# CXX #
	#   $(CXX)
	#
	# OPTIM #
	#   $(OPTIM)
	#
	# INCL #
	#   $(INCL)
	#
	# DEFS #
	#   $(DEFS)
	#


default_test: $(EXE_1) 
	sleep 1 # align with second tick
	./$< tty
	./$< ls -l
	./$< --tee $@.log  tty
	./$< --tee_a $@.log  tty
	./$< false

test1: $(EXE_1) 
	./$(EXE_1) sh -c "sleep 5; false"

gdb_test1: $(EXE_1)
	INGDB=1 gdb -tui --command ../.gdb_init_$(PROG1) --args ./$(EXE_1) false
	#
	#INGDB=1 gdb -tui --command ../.gdb_init_$(PROG1) --args ./$(EXE_1) tty


test99:
	sh -c 'echo $$VPATH'

exe1: $(EXE_1)

default1_test: P $(EXE_1)
	./$<

default2_test: P $(EXE_2)
	./$<


exe: $(EXE_1) $(EXE_2)

P:
	#
	@var_list VPATH
	@var_list PKG_CONFIG_PATH
	@var_list CPLUS_INCLUDE_PATH
	@var_list C_INCLUDE_PATH
	@var_list LIBRARY_PATH
	#
	# The INCLUDE settings come from this commands output
	#
	$(PKG_CONFIG) $(PKGS) --cflags
	#
	# LINKING options
	#
	$(PKG_CONFIG) $(PKGS) --libs
	#
	#

## $(EXE_1): $(OBJS) # NO - now using explicit $(PROG1).o to be sure

$(EXE_1): $(PROG1).o $(OBJS) # make the main first

$(EXE_2): $(PROG2).o # Cdouble main is a problem !

# $(EXE_1): $(OBJS) $(PROG1).o # make the main last!
# that doesnt work - .exe probably depends on .o so its first!
# the benefit is sorting out the synax of a (new) basic .h
# dont believe me ? drop the ';' from a class decl

zap:
	rm -rf $(EXELIST) *.o 

#
# DEPENDENCIES
#

