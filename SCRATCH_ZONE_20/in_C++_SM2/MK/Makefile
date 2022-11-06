#!/usr/bin/make -f
# external users of the libs_apps code can do this:
# include /home/gps/2004/src/libs_apps/MK_ext_lib/MK_with_libs_apps

# rework this to new pmakefile layout
# code borrowed from (ssl_somewhere)
# this is supposed to be SM = Spider Monkey

	# we are in $T/obj/.

	T=..
	# You will probably move this
	YEAR_src_PLAY=$(HOME)/YEAR/src/PLAY
	# subdir in that project where makefiles are
	# which will also need editing (soz)
	MK_ext_lib=$(YEAR_src_PLAY)/libs_apps/MK_ext_lib

	CXX = g++
	OPTIM += -g 

# UNUSED see INCL or todo INCL_LIST
#	INCLS += -I./
#	INCLS += -I../
	INCLS += -I$T/src/
#	INCLS += -I../src/

	DEFS = 

	#
	LIST_VPATH +=$T
	LIST_VPATH +=$T/src
#	LIST_VPATH +=$T/src/some_sub_dir
	LIST_VPATH+=:$T/objs
#	LIST_VPATH+=:.

#	# libs_apps # brings this
#        LIST_PKG_CONFIG_PATH += $(DIR_G_prefix)/lib/pkgconfig/
#        LIST_PKG_CONFIG_PATH += $(DIR_G_prefix)/SM/lib/pkgconfig/

	LIST_RPATH += $(DIR_G_prefix)/SM/lib


#	include ../MK/Makefile_libs_apps # can be a symb link
	include $(MK_ext_lib)/MK_with_libs_apps # TODO put back there
# which includes these
#	include $(MK_ext_lib)/MK_SUFFIXES
#	include $(MK_ext_lib)/MK_LISTS_

# done in MK_with_libs_apps
#	TGT=WIN32
#	TGT=LGX

	PKGS += libs_apps_base1
	PKGS += libs_apps_base2
	PKGS += libs_apps_uses_ssl
	PKGS += libs_apps_mozjs
	# mozjs is included by others anyway

# names of the programs (but not with .exec)

	PROG2 =SM_BASICS# NOSP
	PROG1 =SM_test1# NOSP
	# PROG1 =ssl_bio_test# NOSP

# .exec files are used to make new makefiles easier ? and WIN32 too

	EXE_1 =$(PROG1).exec# NOSP
	EXE_2 =$(PROG2).exec# NOSP

# one way of making all targets

	EXELIST += $(EXE_1)
	EXELIST += $(EXE_2)

#	OBJS += SM_BASICS.o

# no need for PROG1.o
# except when deleting *.o

#
# DEFAULT COMES FIRST
#

default: default_test

defaults2: show_vars

show_vars:
	# NOWT TO DO HERE
	#
	# VPATH
	# $(VPATH)
	#
	# CXX
	# $(CXX)
	#
	# OPTIM
	# $(OPTIM)
	#
	# INCL
	# $(INCL)
	#
	# DEFS
	# $(DEFS)
	#

default_test: $(EXE_2)
	./$<
	# NO # ./client_req_view.sh
	# NOW BUILTIN to save files # ./client_iss_view.sh

CA_ROOT: $(EXE_1)
	./$(EXE_1) CA_ROOT

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
	# PKG_CONFIG_PATH=# dirs used to find pkgconfig files
	# $(PKG_CONFIG_PATH)
	var_list PKG_CONFIG_PATH
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

