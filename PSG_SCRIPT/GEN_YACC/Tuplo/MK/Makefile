#!/usr/bin/env make

	CXX = g++
	OPTIM += -g
	INCLS += -I./
	INCLS += -I../
	INCLS += -I../STO/
#	INCLS += -I../buffer # get from libs_apps
#	INCLS += -I/tools/2000/X11R6/include

	DEFS = 
#	LINKS = -L/usr/X11R6/lib -lX11

	# S=../src
	S=..
	VPATH +=$S
	VPATH +=$S/src
	VPATH +=$S/src/PSG_Tree
	VPATH +=$S/src/STO

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
	e1_psg=../obj/gen_e1
	e1_lex=$(e1_psg)_lex
	e1_yacc=$(e1_psg)_yacc


	# keep files that are between 1 and 2
	# keep files that are between 2 and 3

ifeq (1,1)
	# CLEAN1 += INTO_INTO
	CLEAN1 += $(into_lex_lex)
	CLEAN1 += $(into_yacc_y)
	# CLEAN1 += INTO_DONE
endif

	# Our CXX code PSG_udef generates PSG = FLEX + BISON
	# use PSG_to_GEN_filenames_lex_y

	into_lex_lex=$(e1_lex).lex
	into_yacc_y=$(e1_yacc).y

	# then run FLEX and BISON

#	from_lex=$(e1_lex)
	from_lex_cc=$(e1_lex).cc
	from_lex_o=$(e1_lex).o

	from_yacc_hh=$(e1_yacc).tab.hh
	from_yacc_cc=$(e1_yacc).tab.cc
	from_yacc_o=$(e1_yacc).tab.o

	# then run FLEX and BISON generated code / add to EXE

	OBJS2 += $(from_lex_o)
	OBJS2 += $(from_yacc_o)

	# then Module infrastructure code zones
	# order matters - maybe put into .a to mix it up

	OBJS1 += Tree_PSG_LEX_PUNCT_NAME.o 
	OBJS1 += Tree_PSG_LEX_TOKEN.o
	OBJS1 += Tree_PSG.o
	OBJS1 += Tree_PSG_CALC.o
	OBJS1 += Tree_PSG_UDEF.o
	OBJS1 += Tree_PSG_UDEF_rules.o
	OBJS2 += EXPRS.o
	OBJS2 += EXPRS_RETVAL.o
# NO	OBJS2 += EXPRS_PRINTER.o


	# PGS will want STO but it was itself PROG2
	OBJS2 += sto_4k.o
	OBJS2 += STO.o
	OBJS2 += mmap_file.o

	# clean between phases # maybe

	CLEAN2 += $(from_lex_hh)
	CLEAN2 += $(from_yacc_hh)
	CLEAN2 += $(from_lex_cc)
	CLEAN2 += $(from_yacc_cc)

	CLEAN1 += $(OBJS1)
	CLEAN2 += $(OBJS2)

# END of ASSIGNMENT STYLE STATEMENTS
# START OF Dependency RULES

all: test1 test2

#	helpful retest dependency
#	make the linked.exec depend on Makefile
# $(PROG1_exec): ../MK/Makefile # EG
#	if Makefile edited, relink
#	and then rerun tests that depended on .exec
#	or at least the one you are triggering
#	depends on .exec and reruns soon as

$(PROG1_exec): ../MK/Makefile
$(PROG2_exec): ../MK/Makefile

# SUFFIXES must be in TARGETS section
# you can include it, when you include ../MK/Makefile_SOMETHING

.SUFFIXES: .cxx .tab.c .tab.cc .cc .c .y .o                                                           
# Generate YACC INPUT files (was from tcl)
# e1 # adding _psg _lex to see what_comes from where
#
#	e1_yacc.y
#	e1_lex.lex

# this is test1 - it generates the .lex .y files


# test1 is to run PSG_udef to produce filename .lex .y
#  gen_e1 is the name used in all generated files
#   PSG * e1 = new_PSG_for_math_C_expr(" e1 ");
#   print_e1_for_flex_bison ...

test1: $(into_yacc_y)

# DEPENDECY TREE should be here
# everything depends on  $(into_yacc_y)
# $(into_yacc_y) depends on  PROG1 ; run PROG1 with args

# lex follows yacc, because yacc produces the TABLE and TOKENS
# 
#	$(from_yacc_hh): $(into_yacc_y) 	# .y -> tab.hh
#	$(from_lex_hh): $(into_lex_lex) 	# .lex -> .hh # ??
#	$(into_lex_lex): $(from_yacc_hh)	# run BISON before FLEX

#	$(from_lex_cc): $(from_lex_hh) 	# edit .h triggers own recompile
#	$(from_lex_cc): $(into_yacc_y) 	# everything depends on Tree_top

$(from_yacc_hh): $(into_yacc_y)

# TOP does depend on a few things beyond PROG

$(into_yacc_y): ../src/EXPRS.h
$(into_yacc_y): ../src/e1.y_RULES 

#
# most nearby code want those tokens
# $(into_lex_lex): $(from_yacc_hh)

$(into_lex_lex): $(from_yacc_hh)
$(from_yacc_cc): $(from_yacc_hh)

$(from_lex_cc): $(from_yacc_hh)

$(from_lex_o): $(from_yacc_hh)

unused34:
	# DEPENDENCY ... uses ...
	# $(from_lex_o)
	# $(from_yacc_hh)
	ls -lrt $(from_yacc_hh)
	#


$(into_yacc_y): $(PROG1_exec)
	#
	# GEN
	# RUN $(PROG1_exec)
	# PSG $(e1_psg)
	# adds _lex _yacc .lex .y 
	# later _lex _yacc .lex .y .cc .o _yac.tab.hh 
	# Generate .lex .y files # PSG and LEX # Tree C++ components #
	#
	./$(PROG1_exec) $(e1_psg) #
	#
	# Created
	# $(into_lex_lex)
	# $(into_yacc_y)
	#
	@# ls -l1t $(e1_lex).lex  $(into_yacc_y)
	@ ls -l1t $(e1_lex).lex  $(into_yacc_y)

# YACC OUTPUT files

# both .hh .cc produced at same time by bison
# but want to use $@ to tell bison $(from_yacc_cc)

# $(from_yacc_cc) $(from_yacc_hh):  $(into_yacc_y) 


$(from_yacc_hh): $(from_yacc_cc)

# $(from_yacc_cc): $(from_yacc_hh)
# $(from_yacc_hh): $(into_yacc_y 

$(from_yacc_cc): $(into_yacc_y)
	#
	# BISON
	# FROM     < $<
	# CREATING @ $@
	#
	bison -d -o $@ $<
	#
	# CREATED @ $@
	#

# flex uses bisons tab.hh and the TOKEN lists there
# so make it dependent on the output from BISON from_yacc_hh 
# from_yacc_hh e1_yacc.tab.hh

$(from_lex_cc): $(into_lex_lex) $(from_yacc_hh)
	# e1_lex = $(e1_lex)
	# expect  "gen_e1_lex"
	#
	# FLEX
	# FROM     < $<
	# CREATING @ $@
	#
	flex -o $@ $(e1_lex).lex
	#
	# CREATED @ $@
	#

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
	./$(PROG2_exec) $S/test2.test

dgb_PROG2: $(PROG2_exec)
	# debug
	# BUT STDIN < $S/test2.test
	# errm
	gdb --args ./$(PROG2_exec) $S/test2.test

dgb: $(PROG1_exec)
	# debug
	gdb --args ./$(PROG1_exec) $(e1_psg)

$(PROG1_exec): $(PROG1).o $(OBJS1) 
	g++ $(PROG1).o $(OBJS1) $(LINKS) -o $@

$(PROG2_exec): $(PROG2).o $(OBJS2) 
	# PROG2
	g++ $(PROG2).o $(OBJS2) $(LINKS) -o $@

clean: 
	rm -f $(CLEAN1) $(CLEAN2) 



