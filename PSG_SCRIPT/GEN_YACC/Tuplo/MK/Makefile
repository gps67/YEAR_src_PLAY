#!/usr/bin/env make

	# S=../src
	S=..

	CXX = g++
	OPTIM += -g

	INCLS += -I./
	INCLS += -I../
	INCLS += -I$S/src/
	INCLS += -I$S/src/PSG_Tree
	INCLS += -I$S/src/PSG_Tree/e1
	INCLS += -I$S/src/PSG_Tree/AFM
	INCLS += -I$S/src/PSG_Tree/JSON
	INCLS += -I$S/STO/
#	INCLS += -I../buffer # get from libs_apps
#	INCLS += -I/tools/2000/X11R6/include

	DEFS = 
#	LINKS = -L/usr/X11R6/lib -lX11

	VPATH +=$S
	VPATH +=$S/src
	VPATH +=$S/src/PSG_Tree
	VPATH +=$S/src/PSG_Tree/e1
	VPATH +=$S/src/PSG_Tree/AFM
	VPATH +=$S/src/PSG_Tree/JSON
	VPATH +=$S/src/STO

 #	include ../MK/Makefile_libs_apps
	include ../MK/MK_with_libs_apps

	PKGS += libs_apps_base2

	PROG1=test1_gen_yacc
	PROG2=test2_use_yacc
	PROG3=test3_use_yacc
	PROG4=test4_use_yacc_on_JSON

	PROG1_exec=$(PROG1).exec
	PROG2_exec=$(PROG2).exec
	PROG3_exec=$(PROG3).exec
	PROG4_exec=$(PROG4).exec

	CLEAN1 += $(PROG1).o
	CLEAN2 += $(PROG2).o
	CLEAN3 += $(PROG3).o
	CLEAN4 += $(PROG4).o

	CLEAN1 += $(PROG1_exec)
	CLEAN2 += $(PROG2_exec)
	CLEAN3 += $(PROG3_exec)
	CLEAN4 += $(PROG4_exec)

	# PROG knows to add _yacc _lex
	# MAKE still needs to know
	# add lines for each gramma
	# track use around Makefile
	#
	e1_psg_2=AFM
	e1_psg_3=e1
	e1_psg_4=JSON

	# logic that is in source, names of files generated, ../obj/gen_*
	e1_gen=gen_
	e1_left=../obj/
	e1_file_2=$(e1_left)$(e1_gen)$(e1_psg_2)# NOSP # same logic in binary
	e1_file_3=$(e1_left)$(e1_gen)$(e1_psg_3)# NOSP # same logic in binary
	e1_file_4=$(e1_left)$(e1_gen)$(e1_psg_4)# NOSP # 
	e1_lex_2=$(e1_file_2)_lex
	e1_lex_3=$(e1_file_3)_lex
	e1_lex_4=$(e1_file_4)_lex
	e1_yacc_2=$(e1_file_2)_yacc
	e1_yacc_3=$(e1_file_3)_yacc
	e1_yacc_4=$(e1_file_4)_yacc


	# keep files that are between 1 and 2
	# keep files that are between 2 and 3

ifeq (1,1)
	# CLEAN1 += INTO_INTO
	CLEAN1 += $(into_lex_lex_2)
	CLEAN1 += $(into_yacc_y_2)
	# CLEAN1 += INTO_DONE
endif

ifeq (1,1)
	CLEAN2 += $(into_lex_lex_3)
	CLEAN2 += $(into_yacc_y_3)
endif

ifeq (1,1)
	CLEAN3 += $(into_lex_lex_4)
	CLEAN3 += $(into_yacc_y_4)
endif

	# Our CXX code PSG_udef generates PSG = FLEX + BISON
	# use PSG_to_GEN_filenames_lex_y

	into_lex_lex_2=$(e1_lex_2).lex
	into_lex_lex_3=$(e1_lex_3).lex
	into_lex_lex_4=$(e1_lex_4).lex
	into_yacc_y_2=$(e1_yacc_2).y
	into_yacc_y_3=$(e1_yacc_3).y
	into_yacc_y_4=$(e1_yacc_4).y

	gen_list += $(into_lex_lex_2)
	gen_list += $(into_lex_lex_3)
	gen_list += $(into_lex_lex_4)
	gen_list += $(into_yacc_y_2)
	gen_list += $(into_yacc_y_3)
	gen_list += $(into_yacc_y_4)



	# then run FLEX and BISON

#	from_lex=$(e1_lex_2)
	from_lex_cc_2=$(e1_lex_2).cc
	from_lex_cc_3=$(e1_lex_3).cc
	from_lex_cc_4=$(e1_lex_4).cc
	from_lex_o_2=$(e1_lex_2).o
	from_lex_o_3=$(e1_lex_3).o
	from_lex_o_4=$(e1_lex_4).o

	from_yacc_hh_2=$(e1_yacc_2).tab.hh
	from_yacc_hh_3=$(e1_yacc_3).tab.hh
	from_yacc_hh_4=$(e1_yacc_4).tab.hh
	from_yacc_cc_2=$(e1_yacc_2).tab.cc
	from_yacc_cc_3=$(e1_yacc_3).tab.cc
	from_yacc_cc_4=$(e1_yacc_4).tab.cc
	from_yacc_o_2=$(e1_yacc_2).tab.o
	from_yacc_o_3=$(e1_yacc_3).tab.o
	from_yacc_o_4=$(e1_yacc_4).tab.o

	# then run FLEX and BISON generated code / add to EXE

	OBJS2 += $(OBJS23)
	OBJS3 += $(OBJS23)
	OBJS4 += $(OBJS23)

	# $(OBJS24)

	OBJS2 += $(from_lex_o_2)
	OBJS3 += $(from_lex_o_3)
	OBJS4 += $(from_lex_o_4)

	OBJS2 += $(from_yacc_o_2)
	OBJS3 += $(from_yacc_o_3)
	OBJS4 += $(from_yacc_o_4)

	# then Module infrastructure code zones
	# order matters - maybe put into .a to mix it up

	OBJS1 += lex_yacc_LEX_PUNCT_NAME.o 
	OBJS1 += lex_yacc_LEX_TOKEN.o
	OBJS1 += lex_yacc_LEX_TOKEN_precedence.o
	OBJS1 += lex_yacc.o
	OBJS1 += lex_yacc_UDEF.o
	OBJS1 += lex_yacc_UDEF_C_EXPR_lex.o
	OBJS1 += lex_yacc_UDEF_C_EXPR_yacc.o
	OBJS1 += lex_yacc_UDEF_CALC.o
	OBJS1 += lex_yacc_UDEF_AFM.o
	OBJS1 += lex_yacc_UDEF_JSON.o
	OBJS1 += STO_E32.o
	OBJS1 += Y_PARSE_gen.o 
	OBJS1 += lex_yacc_LEX_START.o 
	# OBJS1 is specific to JSON etc - rework required ...
	# Y_PARSE is to become common to all
	OBJS23 += Y_PARSE.o 
	OBJS23 += EXPRS.o
	OBJS23 += EXPRS_RETVAL.o
# NO	OBJS2 += EXPRS_PRINTER.o


	# PGS will want STO but it was itself PROG2
	OBJS2 += sto_4k.o
	OBJS2 += STO.o
	OBJS2 += mmap_file.o

	# clean between phases # maybe

	CLEAN2 += $(from_lex_hh_2)
	CLEAN2 += $(from_yacc_hh_2)
	CLEAN2 += $(from_lex_cc_2)
	CLEAN2 += $(from_yacc_cc_2)

	CLEAN3 += $(from_lex_hh_3)
	CLEAN3 += $(from_yacc_hh_3)
	CLEAN3 += $(from_lex_cc_3)
	CLEAN3 += $(from_yacc_cc_3)

	CLEAN4 += $(from_lex_hh_4)
	CLEAN4 += $(from_yacc_hh_4)
	CLEAN4 += $(from_lex_cc_4)
	CLEAN4 += $(from_yacc_cc_4)

	CLEAN1 += $(OBJS1)
	CLEAN2 += $(OBJS2)
	CLEAN3 += $(OBJS3)
	CLEAN4 += $(OBJS4)

# END of ASSIGNMENT STYLE STATEMENTS
# START OF Dependency RULES

#all: test4
# all: test2
all: test1 test2 test3 test4

#	helpful retest dependency
#	make the linked.exec depend on Makefile
# $(PROG1_exec): ../MK/Makefile # EG
#	if Makefile edited, relink
#	and then rerun tests that depended on .exec
#	or at least the one you are triggering
#	depends on .exec and reruns soon as

$(PROG1_exec): ../MK/Makefile
$(PROG2_exec): ../MK/Makefile
$(PROG3_exec): ../MK/Makefile
$(PROG4_exec): ../MK/Makefile

# SUFFIXES must be in TARGETS section
# you can include it, when you include ../MK/Makefile_SOMETHING

.SUFFIXES: .cxx .tab.c .tab.cc .cc .c .lex .y .o

# Generate YACC INPUT files (was from tcl)
# e1 # adding _psg _lex to see what_comes from where
#
#	e1_yacc_2.y
#	e1_lex_2.lex

# this is test1 - it generates the .lex .y files


# test1 is to run PSG_udef to produce filename .lex .y
#  gen_e1 is the name used in all generated files
#   PSG * e1 = new_PSG_for_math_C_expr(" e1 ");
#   print_e1_for_flex_bison ...

test1: $(into_yacc_y_2)

# DEPENDECY TREE should be here
# everything depends on  $(into_yacc_y_2)
# $(into_yacc_y_2) depends on  PROG1 ; run PROG1 with args

# lex follows yacc, because yacc produces the TABLE and TOKENS
# 
#	$(from_yacc_hh_2): $(into_yacc_y_2) 	# .y -> tab.hh
#	$(from_lex_hh_2): $(into_lex_lex_2) 	# .lex -> .hh # ??
#	$(into_lex_lex_2): $(from_yacc_hh_2)	# run BISON before FLEX

#	$(from_lex_cc_2): $(from_lex_hh_2) 	# edit .h triggers own recompile
#	$(from_lex_cc_2): $(into_yacc_y_2) 	# everything depends on Tree_top

$(from_yacc_hh_2): $(into_yacc_y_2)
$(from_yacc_hh_3): $(into_yacc_y_3)
$(from_yacc_hh_4): $(into_yacc_y_4)

# TOP does depend on a few things beyond PROG

$(into_yacc_y_2): ../src/EXPRS.h
$(into_yacc_y_2): ../src/e1.y_RULES 
$(into_yacc_y_3): ../src/AFM.y_RULES 
$(into_yacc_y_4): ../src/JSON.y_RULES 

$(PROG1_exec): ../src/AFM.y_RULES 
$(PROG4_exec): ../src/JSON.y_RULES 

#
# most nearby code want those tokens
# $(into_lex_lex_2): $(from_yacc_hh_2)

$(into_lex_lex_2): $(from_yacc_hh_2)
$(into_lex_lex_3): $(from_yacc_hh_3)
$(into_lex_lex_4): $(from_yacc_hh_4)
$(from_yacc_cc_2): $(from_yacc_hh_2)
$(from_yacc_cc_3): $(from_yacc_hh_3)
$(from_yacc_cc_4): $(from_yacc_hh_4)

$(from_lex_cc_2): $(from_yacc_hh_2)
$(from_lex_cc_3): $(from_yacc_hh_3)
$(from_lex_cc_4): $(from_yacc_hh_4)

$(from_lex_o_2): $(from_yacc_hh_2)
$(from_lex_o_3): $(from_yacc_hh_3)
$(from_lex_o_4): $(from_yacc_hh_4)


# fix
EXPRS.o: $(from_yacc_hh_3)

unused34:
	# DEPENDENCY ... uses ...
	# $(from_lex_o_2)
	# $(from_yacc_hh_2)
	ls -lrt $(from_yacc_hh_2)
	#


$(into_yacc_y_2): $(PROG1_exec)
	#
	# GEN 2 $@
	# RUN $(PROG1_exec)
	# LEFT $(e1_left)
	# GEN_ $(e1_gen)
	# PSG $(e1_psg_2)
	# adds _lex _yacc .lex .y 
	# later _lex _yacc .lex .y .cc .o _yac.tab.hh 
	# Generate .lex .y files # PSG and LEX # Tree C++ components #
	#
	./$(PROG1_exec) $(e1_left) $(e1_gen) $(e1_psg_2) #
	#
	# Created
	# $(into_lex_lex_2)
	# $(into_yacc_y_2)
	#
	@# ls -l1t $(e1_lex_2).lex  $(into_yacc_y_2)
	@ ls -l1t $(e1_lex_2).lex  $(into_yacc_y_2)

$(into_yacc_y_3): $(PROG1_exec)
	#
	# GEN 3 $@
	# RUN $(PROG1_exec)
	# LEFT $(e1_left)
	# GEN_ $(e1_gen)
	# PSG $(e1_psg_3)
	# adds _lex _yacc .lex .y 
	# later _lex _yacc .lex .y .cc .o _yac.tab.hh 
	# Generate .lex .y files # PSG and LEX # Tree C++ components #
	#
	./$(PROG1_exec) $(e1_left) $(e1_gen) $(e1_psg_3) #
	#
	# Created
	# $(into_lex_lex_3)
	# $(into_yacc_y_3)
	#
	@# ls -l1t $(e1_lex_3).lex  $(into_yacc_y_3)
	@ ls -l1t $(e1_lex_3).lex  $(into_yacc_y_3)

$(into_yacc_y_4): $(PROG1_exec)
	#
	# GEN 4 $@
	# RUN $(PROG1_exec)
	# LEFT $(e1_left)
	# GEN_ $(e1_gen)
	# PSG $(e1_psg_4)
	# adds _lex _yacc .lex .y 
	# later _lex _yacc .lex .y .cc .o _yac.tab.hh 
	# Generate .lex .y files # PSG and LEX # Tree C++ components #
	#
	./$(PROG1_exec) $(e1_left) $(e1_gen) $(e1_psg_4) #
	#
	# Created
	# $(into_lex_lex_4)
	# $(into_yacc_y_4)
	#
	@# ls -l1t $(e1_lex_4).lex  $(into_yacc_y_4)
	@ ls -l1t $(e1_lex_4).lex  $(into_yacc_y_4)

# YACC OUTPUT files

# both .hh .cc produced at same time by bison
# but want to use $@ to tell bison $(from_yacc_cc_2)

# $(from_yacc_cc_2) $(from_yacc_hh_2):  $(into_yacc_y_2) 


$(from_yacc_hh_2): $(from_yacc_cc_2)
$(from_yacc_hh_3): $(from_yacc_cc_3)
$(from_yacc_hh_4): $(from_yacc_cc_4)

# $(from_yacc_cc_2): $(from_yacc_hh_2)
# $(from_yacc_hh_2): $(into_yacc_y_2 

$(from_yacc_cc_2): $(into_yacc_y_2)
	#
	# BISON
	# FROM     < $<
	# CREATING @ $@
	#
	bison -d -o $@ $<
	#
	# CREATED @ $@
	#

$(from_yacc_cc_3): $(into_yacc_y_3)
	#
	# BISON
	# FROM     < $<
	# CREATING @ $@
	#
	bison -d -o $@ $<
	#
	# CREATED @ $@
	#

$(from_yacc_cc_4): $(into_yacc_y_4)
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
# so make it dependent on the output from BISON from_yacc_hh_2 
# from_yacc_hh_2 e1_yacc_2.tab.hh

$(from_lex_cc_2): $(into_lex_lex_2) $(from_yacc_hh_2)
	# e1_lex_2 = $(e1_lex_2)
	# expect  "gen_e1_lex"
	#
	# FLEX
	# FROM     < $<
	# CREATING @ $@
	#
	flex -o $@ $(e1_lex_2).lex
	#
	# CREATED @ $@
	#

$(from_lex_cc_3): $(into_lex_lex_3) $(from_yacc_hh_3)
	# e1_lex_3 = $(e1_lex_3)
	# expect  "gen_e1_lex"
	#
	# FLEX
	# FROM     < $<
	# CREATING @ $@
	#
	flex -o $@ $(e1_lex_3).lex
	#
	# CREATED @ $@
	#

$(from_lex_cc_4): $(into_lex_lex_4) $(from_yacc_hh_4)
	# e1_lex_4 = $(e1_lex_4)
	# expect  "gen_e1_lex"
	#
	# FLEX
	# FROM     < $<
	# CREATING @ $@
	#
	flex -o $@ $(e1_lex_4).lex
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

test3: $(PROG3_exec)
	# test3
	./$(PROG3_exec) $S/test3.test

test4: $(PROG4_exec)
	# test4
	./$(PROG4_exec) $S/test4.test

dgb_PROG4: $(PROG4_exec)
	# debug
	# BUT STDIN < $S/test4.test
	# errm
	# gdb --args ./$(PROG4_exec)
	#
	# -tui messes stderr with same tty
	gdb -tui --command ../.gdbinit_$(PROG4) --args ./$(PROG4_exec) $S/test4.test

dgb_PROG3: $(PROG3_exec)
	# debug
	# BUT STDIN < $S/test3.test
	# errm
	# gdb --args ./$(PROG3_exec)
	#
	# -tui messes stderr with same tty
	gdb -tui --command ../.gdbinit_$(PROG3) --args ./$(PROG3_exec)

dgb_PROG2: $(PROG2_exec)
	# debug
	# BUT STDIN < $S/test2.test
	# errm
	gdb -tui --command ../.gdbinit_$(PROG2) --args ./$(PROG2_exec) $S/test2.test

dgb_PROG1: $(PROG1_exec)
	# debug
	gdb -tui --command ../.gdbinit_$(PROG1) --args ./$(PROG1_exec) $(e1_left) $(e1_gen) $(e1_psg_2)

dgb_insight: $(PROG1_exec)
	# debug
	insight --args ./$(PROG1_exec) $(e1_left) $(e1_gen) $(e1_psg_2)

$(PROG1_exec): $(PROG1).o $(OBJS1) 
	# PROG1 - GEN -
	g++ $(PROG1).o $(OBJS1) $(LINKS) -o $@

$(PROG2_exec): $(PROG2).o $(OBJS2) 
	# PROG2 AFM
	g++ $(PROG2).o $(OBJS2) $(LINKS) -o $@

$(PROG3_exec): $(PROG3).o $(OBJS3) 
	# PROG3 CALC
	g++ $(PROG3).o $(OBJS3) $(LINKS) -o $@

$(PROG4_exec): $(PROG4).o $(OBJS4) 
	# PROG4 JSON
	g++ $(PROG4).o $(OBJS4) $(LINKS) -o $@

T4: $(PROG4_exec)
	# OK

CLEAN1: 
	# clean 1
	rm -f $(CLEAN1) 

clean: 
	# clean 
	rm -f $(CLEAN1) $(CLEAN2) $(CLEAN3) $(CLEAN4) 

clean_gen:
	# clean_gen
	# the generated files # list of
	rm -f $(gen_list)



