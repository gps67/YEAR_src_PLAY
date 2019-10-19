#!/usr/bin/env make

	CXX = g++
	OPTIM += -g
	INCLS += -I./
	INCLS += -I../
#	INCLS += -I../buffer # get from libs_apps
#	INCLS += -I/tools/2000/X11R6/include

	DEFS = 
#	LINKS = -L/usr/X11R6/lib -lX11

	# S=../src
	S=..
	VPATH +=$S
	VPATH +=$S/src
	VPATH +=$S/src/PSG_Tree

	include ../MK/Makefile_libs_apps

	PROG1=test1_gen_yacc
	PROG1_exec=$(PROG1).exec

	PROG2=test2_use_yacc
	PROG2_exec=$(PROG2).exec

	CLEAN1 += $(PROG1).o
	CLEAN2 += $(PROG2).o

	CLEAN1 += $(PROG1_exec)
	CLEAN2 += $(PROG2_exec)

	# PROG knows to add _yacc _lex
	# MAKE still needs to know
	#
	e1_psg=gen_e1
	e1_lex=$(e1_psg)_lex
	e1_yacc=$(e1_psg)_yacc


	# keep files that are between 1 and 2
	# keep files that are between 2 and 3

#	gen_lex=$S/$(e1_lex).lex
#	gen_yacc=$S/$(e1_yacc).y
#	CLEAN12 += $(gen_lex)
#	CLEAN12 += $(gen_yacc)

	# yacc and lex output different names .tab.

	from_lex=$(e1_lex)
	from_lex_cc=$(e1_lex).cc
	from_lex_o=$(e1_lex).o

	from_yacc_hh=$(e1_yacc).tab.hh
	from_yacc_cc=$(e1_yacc).tab.cc
	from_yacc_o=$(e1_yacc).tab.o

	OBJS2 += $(from_lex_o)
	OBJS2 += $(from_yacc_o)

	# order matters - maybe put into .a to mix it up
	OBJS1 += Tree_PSG_LEX_PUNCT_NAME.o 
	OBJS1 += Tree_PSG_LEX_TOKEN.o
	OBJS1 += Tree_PSG.o
	OBJS1 += Tree_PSG_UDEF.o

	CLEAN2 += $(from_lex_hh)
	CLEAN2 += $(from_yacc_hh)
	CLEAN2 += $(from_lex_cc)
	CLEAN2 += $(from_yacc_cc)

	CLEAN1 += $(OBJS1)
	CLEAN2 += $(OBJS2)

all: test1 test2

.SUFFIXES: .cxx .tab.c .tab.cc .cc .c .y .o                                                           
# Generate YACC INPUT files (was from tcl)
# e1 # adding _psg _lex to see what_comes from where
#
#	e1_yacc.y
#	e1_lex.lex

# this is test1 - it generates the .lex .y files

test1: $S/$(e1_lex).lex $S/$(e1_yacc).y 

$S/$(e1_lex).lex $S/$(e1_yacc).y: $(PROG1_exec)
	#
	# Generate .lex .y files # PSG and LEX # Tree C++ components #
	#
	./$(PROG1_exec) $(e1_psg) # NO MORE # $(e1_lex) .lex $(e1_yacc).y
	#

#########
unused:
	# BISON .yac script # Makeilfe
	# FLEX .lex script # uses tokens from yac
	# NEXT GET Name.cc .hh 
	# NEXT GET Name.o Prog.exec
	# NEXT RUN        Prog.exec Data.sample
	# NEXT FILE LexName.tab.hh
	# NEXT FILE $(from_yacc_hh)
	# NEXT FILE $(from_yacc_cc)
	# NEXT FILE $(from_yacc_o)
	# NEXT Maybe Deferred
	# NEXT Option RUN Filter over DATA using SCRIPTING_POOL
	#
	# #


# YACC OUTPUT files

# both .hh .cc produced at same time by bison
# but want to use $@ to tell bison $(from_yacc_cc)

$(from_yacc_cc) $(from_yacc_hh): $(e1_yacc).y 
	#
	# BISON
	# CREATING $@
	# FROM < $<
	bison -d -o $@ $<


unused:
	# NOT # bison -o $@ -L c++ $<

# flex uses bisons tab.hh and the TOKEN lists there
# so make it dependent on the output from BISON from_yacc_hh 
# from_yacc_hh e1_yacc.tab.hh

$(from_lex_cc): $(e1_lex).lex $(from_yacc_hh)
	pwd
	# e1_lex = $(e1_lex) # gen_e1_lex
	#
	# FLEX
	# CREATING $@
	# FROM < $ <
	# FROM ^ $ ^
	# NOT # flex -o $@ $^
	flex -o $@ $(e1_lex).lex
	# DONR MENTION $(from_yacc_hh)

# CXX files

# all C files get all flags
# when live both will be same ??
# whan live may need loadable filter

.cc.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  

.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS1): $(HDRS)
$(OBJS2): $(HDRS)

#test1: $(PROG1_exec)
#	./$(PROG1_exec) gen

test2: $(PROG2_exec)
	# test2
	./$(PROG2_exec) < $S/test2.test

dgb: $(PROG1_exec)
	# debug
	gdb --args ./$(PROG1_exec) $(e1_psg)
#	gdb --args ./$(PROG1_exec) $(e1_lex).lex $(e1_yacc).y
#	gdb --args ./$(PROG2_exec) < $S/test2.test

$(PROG1_exec): $(PROG1).o $(OBJS1) 
	# g++ $< $(LINKS) -o $@
	g++ $(PROG1).o $(OBJS1) $(LINKS) -o $@

$(PROG2_exec): $(PROG2).o $(OBJS2) 
	# g++ $< $(LINKS) -o $@
	g++ $(PROG2).o $(OBJS2) $(LINKS) -o $@

clean: 
	rm -f $(CLEAN1) $(CLEAN2) 
	# rm -f $(CLEAN12) $(CLEAN23) 



