#!/usr/bin/env make

	# uses libs_apps #
	# uses # libs_apps_mozjs.pc # Requires: mozjs-112 #
# WAX
	## hmmm # now dependent on libs_apps

	CXX = g++
	OPTIM += -g

	# pick .exec or .elf #
	# nonstandard, but makes rules easier #
	dot_EXE=.elf
#	dot_EXE=.exec

	# compile in ../obj
	# sources in ../src
	# needs to have srcfile.cxx to know to build srcfile.o .elf
	# also if there is no PROG4.cxx # PROG4.exec will not be known as old
	# so this would attempt to run an absent PROG4.exec
	S=../src
	VPATH += $S 

# rebuild everything after editing ANY makefile !!

	MAKES += ../MK/MK_with_libs_apps
	MAKES += ../MK/Makefile

# build using libs_apps # base1 create this symb-link and include it
# you still need # PKGS += libs_apps_base1

	include ../MK/MK_with_libs_apps

# want RULE .o depends on .h
# this makes ALL .o's dependent on ALL .h's

#	# UNUSED OPTIONS # maybe used via MK_with
#	G_prefix=/home/gps/G/prefix
#	LIST_PKG_CONFIG_PATH += $(G_prefix)/lib/pkgconfig/
#	LIST_RPATH += $(G_prefix)/lib

# PKGS # list of things for pkgconfig 

	PKGS += libs_apps_base1
	PKGS += libs_apps_mozjs

# DEFS 	# -Dname=val #

	DEFS += 


# LINKS # any extra link flags

	LINKS += -rdynamic # export all symbols - backtrace works!

#	LINKS += -L/usr/X11R6/lib -lX11
#	LINKS = -L/tools/2000/X11R6/lib -lX11

	# after any HRDS edited, rebuild OBJS in this order

# OBJS # OBJS += myfile.o # all except PROG.o # that is auto from _SUFFIXES
	

#	OBJS += my_won_classes.o

# PROG1 #

	OBJS += audacity_mod_script_pipe.o

	PROG1=audacity_mod_script_pipe_test
	PROG2=AU_two
	PROG3=AU_3
	PROG4=AU_4

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)
	EXEC3=$(PROG3)$(dot_EXE)
	EXEC4=$(PROG4)$(dot_EXE)
#	gdb=gdb
	gdb=/tools/x86_64/ccs/bin/gdb

# TEST ARGS #

	test1_args = t1_arg1
	test2_args = t2_arg1
	test3_args = t3_arg1
	test4_args = one two 

# DEFAULT is first rule # whic is not really all tho

all: run_test1
# all: lots run_test1
# all: run_test2

## .SUFFIXES: .cxx .c .o # see libs_apps/MK_ext_lib/
##
##	include ../MK/MK_with_libs_apps
##	include .../MK_SUFFIXES
##	include .../MK_LISTS
#
# MK_SUFFIXES defines these RULES which uses these VARS
#
# .cxx.o:
# 	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  
# .c.o:
#	$(GCC) -ditto-
# .o.elf
#	$(CXX) $(OPTIM) -o $@ $< $(OBJS) $(LINKS) $(LIBS)

# NOT SURE but I think you can add a dependency
# $(EXEC1): anextra.o
# and it is added to $< # check this #
# otherwise add it to # OBJS += anextra.o # and everybody gets it



# editing ANY of $(HDRS) causes all $(OBJS) to be rebuilt
# normally uncommented
# HINT # for IDE rebuild the one that failed recently first
# $(OBJS): $(HDRS)
# $(OBJS): $(HDRS)

gdb_test1: $(EXEC1)
	INGDB=1 $(gdb) -tui --command ../.gdb_init_$(PROG1) --args ./$(EXEC1) $(test1_args)  

gdb_test2: $(EXEC2)
	INGDB=1 $(gdb) -tui --command ../.gdb_init_$(PROG2) --args ./$(EXEC2) $(test2_args)  P 

gdb_test3: $(EXEC3)
	INGDB=1 $(gdb) -tui --command ../.gdb_init_$(PROG3) --args ./$(EXEC3) $(test3_args) 

gdb_test4: $(EXEC4)
	INGDB=1 $(gdb) -tui --command ../.gdb_init_$(PROG4) --args ./$(EXEC4) $(test4_args) 


run_test1: $(EXEC1)
	./$(EXEC1) $(test1_args)

run_test2: $(EXEC2)
	./$(EXEC2) $(test2_args)

run_test3: $(EXEC3)
	./$(EXEC3)  $(test3_args)

run_test4: $(EXEC4)
	./$(EXEC4)  $(test4_args)


lots: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) 
	# make lots


$(EXEC1): $(OBJS)
$(EXEC2): $(OBJS)
$(EXEC3): $(OBJS)
$(EXEC4): $(OBJS)

relink: 
	rm -f $(EXEC1)  $(EXEC2)  $(EXEC3)  $(EXEC4)  test2 fa

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2)  $(EXEC3)  $(EXEC4)  test2 fa


