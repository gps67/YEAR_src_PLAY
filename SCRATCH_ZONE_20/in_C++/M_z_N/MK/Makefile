#!/usr/bin/env make

# WAX
	## hmmm # now dependent on libs_apps

	CXX = g++
	OPTIM += -g

	# nonstandard, but makes rules easier
	dot_EXE=.exec

	PKGS += libs_apps_base1

##	LINKS += -rdynamic # export all symbols - backtrace works!

	include ../MK/MK_with_libs_apps

# rebuild everything after editing ANY makefile !!

	MAKES += ../MK/MK_with_libs_apps
	MAKES += ../MK/Makefile

# want RULE .o depends on .h
# this makes ALL .o's dependent on ALL .h's

	HDRS += ../src/M_z_N.h

	# after any HRDS edited, rebuild OBJS in this order

	OBJS += ..M_z_N.o

	PROG1=M_z_N
	PROG2=M_z_N
	PROG3=M_z_N
	PROG4=M_z_N
	PROG5=M_z_N

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)
	EXEC3=$(PROG3)$(dot_EXE)
	EXEC4=$(PROG4)$(dot_EXE)
	EXEC5=$(PROG5)$(dot_EXE)
#	gdb=gdb
	gdb=/tools/x86_64/ccs/bin/gdb

	EXECS += $(EXEC1)
	EXECS += $(EXEC2)
	EXECS += $(EXEC3)
	EXECS += $(EXEC4)
	EXECS += $(EXEC5)

all: run_test1


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

run_test5: $(EXEC5)
	./$(EXEC5) 

lots: $(EXEC1) $(EXEC2) $(EXEC3) 
	# make lots

# pick up default .o.elf from MK_SUFFIXES from libs_apps
# but add .elf depends on ... same massive pool of $(OBJS)
# roll back into libs_apps as .so # LATER #

$(EXEC1): $(OBJS)
$(EXEC2): $(OBJS)
$(EXEC3): $(OBJS)
$(EXEC4): $(OBJS)
$(EXEC5): $(OBJS)

relink: 
	rm -f $(EXECS) test2 fa

clean: 
	rm -f *.o $(EXECS) test2 fa
