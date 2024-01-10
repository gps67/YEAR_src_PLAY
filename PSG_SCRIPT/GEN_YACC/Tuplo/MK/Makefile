#!/usr/bin/env make

	# S=../src
	S=..

	T=../test

	CXX = g++
	OPTIM += -g

	# NAME of PSG Grammar1, PROG and TEST DATA
	#
	# BEWARE no SPACES get into VARS
	G1=e1
	G2=AFM
	G3=JSON

	LIST_CPLUS_INCLUDE_PATH += ./
	LIST_CPLUS_INCLUDE_PATH += ../
	LIST_CPLUS_INCLUDE_PATH += $S/src/
	LIST_CPLUS_INCLUDE_PATH += $S/src/PSG_Tree
	LIST_CPLUS_INCLUDE_PATH += $S/src/PSG_Tree/$(G1)
	LIST_CPLUS_INCLUDE_PATH += $S/src/PSG_Tree/$(G2)
	LIST_CPLUS_INCLUDE_PATH += $S/src/PSG_Tree/$(G3)
	LIST_CPLUS_INCLUDE_PATH += $S/src/lib_YY_parse
	LIST_CPLUS_INCLUDE_PATH += $S/STO/

	DEFS = 

	VPATH +=$S
	VPATH +=$S/src
	VPATH +=$S/src/PSG_Tree
	VPATH +=$S/src/PSG_Tree/$(G1)
	VPATH +=$S/src/PSG_Tree/$(G2)
	VPATH +=$S/src/PSG_Tree/$(G3)
	VPATH +=$S/src/STO
	VPATH +=$S/src/lib_YY_parse

 #	libs_apps
	include ../MK/MK_with_libs_apps

	PKGS += libs_apps_base2

	PROG0=test0_gen_yacc
	PROG1=test1_use_yacc_on_$(G1)
	PROG2=test2_use_yacc_on_$(G2)
	PROG3=test3_use_yacc_on_$(G3)

	PROG0_exec=$(PROG0).exec
	PROG1_exec=$(PROG1).exec
	PROG2_exec=$(PROG2).exec
	PROG3_exec=$(PROG3).exec

	CLEAN0 += $(PROG0).o
	CLEAN1 += $(PROG1).o
	CLEAN2 += $(PROG2).o
	CLEAN3 += $(PROG3).o

	CLEAN0 += $(PROG0_exec)
	CLEAN1 += $(PROG1_exec)
	CLEAN2 += $(PROG2_exec)
	CLEAN3 += $(PROG3_exec)

	# logic that is in source, names of files generated, ../obj/gen_*
	GEN_gen=gen_
	GEN_left=../obj/
	GEN_file_1=$(GEN_left)$(GEN_gen)$(G1)# NOSP # 
	GEN_file_2=$(GEN_left)$(GEN_gen)$(G2)# NOSP # same logic in binary
	GEN_file_3=$(GEN_left)$(GEN_gen)$(G3)# NOSP # same logic in binary

	GEN_lex_1=$(GEN_file_1)_lex
	GEN_lex_2=$(GEN_file_2)_lex
	GEN_lex_3=$(GEN_file_3)_lex

	GEN_yacc_1=$(GEN_file_1)_yacc
	GEN_yacc_2=$(GEN_file_2)_yacc
	GEN_yacc_3=$(GEN_file_3)_yacc

	# keep files that are between 1 and 2
	# keep files that are between 2 and 3

	CLEAN1 += $(into_lex_lex_1)
	CLEAN2 += $(into_lex_lex_2)
	CLEAN3 += $(into_lex_lex_3)
	CLEAN1 += $(into_yacc_y_1)
	CLEAN2 += $(into_yacc_y_2)
	CLEAN3 += $(into_yacc_y_3)

	# Our CXX code PSG_udef generates PSG = FLEX + BISON
	# use PSG_to_GEN_filenames_lex_y

	into_lex_lex_1=$(GEN_lex_1).lex
	into_lex_lex_2=$(GEN_lex_2).lex
	into_lex_lex_3=$(GEN_lex_3).lex

	into_yacc_y_1=$(GEN_yacc_1).y
	into_yacc_y_2=$(GEN_yacc_2).y
	into_yacc_y_3=$(GEN_yacc_3).y

	# keep these after make clean ?
	# actually include in GIT or TAR ?

	gen_list += $(into_lex_lex_1)
	gen_list += $(into_lex_lex_2)
	gen_list += $(into_lex_lex_3)
	gen_list += $(into_yacc_y_1)
	gen_list += $(into_yacc_y_2)
	gen_list += $(into_yacc_y_3)

	# then run FLEX and BISON

#	from_lex=$(GEN_lex_1)
	from_lex_cc_1=$(GEN_lex_1).cc
	from_lex_cc_2=$(GEN_lex_2).cc
	from_lex_cc_3=$(GEN_lex_3).cc

	from_lex_o_1=$(GEN_lex_1).o
	from_lex_o_2=$(GEN_lex_2).o
	from_lex_o_3=$(GEN_lex_3).o

	from_yacc_hh_1=$(GEN_yacc_1).tab.hh
	from_yacc_hh_2=$(GEN_yacc_2).tab.hh
	from_yacc_hh_3=$(GEN_yacc_3).tab.hh

	from_yacc_cc_1=$(GEN_yacc_1).tab.cc
	from_yacc_cc_2=$(GEN_yacc_2).tab.cc
	from_yacc_cc_3=$(GEN_yacc_3).tab.cc

	from_yacc_o_1=$(GEN_yacc_1).tab.o
	from_yacc_o_2=$(GEN_yacc_2).tab.o
	from_yacc_o_3=$(GEN_yacc_3).tab.o

	# then run FLEX and BISON generated code / add to EXE

	OBJS1 += $(OBJS123)
	OBJS2 += $(OBJS123)
	OBJS3 += $(OBJS123)

	# $(OBJS24)

	OBJS1 += $(from_lex_o_1)
	OBJS2 += $(from_lex_o_2)
	OBJS3 += $(from_lex_o_3)

	OBJS1 += $(from_yacc_o_1)
	OBJS2 += $(from_yacc_o_2)
	OBJS3 += $(from_yacc_o_3)

	# then Module infrastructure code zones
	# order matters - maybe put into .a to mix it up

	OBJS0 += lex_yacc_LEX_PUNCT_NAME.o 
	OBJS0 += lex_yacc_LEX_TOKEN.o
	OBJS0 += lex_yacc_LEX_TOKEN_precedence.o
	OBJS0 += lex_yacc_UNION.o
	OBJS0 += lex_yacc.o
	OBJS0 += lex_yacc_UDEF.o
	OBJS0 += lex_yacc_UDEF_C_EXPR_lex.o
	OBJS0 += lex_yacc_UDEF_C_EXPR_yacc.o
	OBJS0 += lex_yacc_UDEF_CALC.o

	OBJS0 += STO_E32.o
	OBJS0 += YY_PARSE_gen.o 
	OBJS0 += lex_yacc_LEX_START.o 

	# prog0 should have scripted decl of PSG and not need this
	OBJS0 += lex_yacc_UDEF_$(G1).o
	OBJS0 += lex_yacc_UDEF_$(G2).o
	OBJS0 += lex_yacc_UDEF_$(G3).o

	# these should not be in 0
	OBJS123 += EXPRS_RETVAL.o
# NO	OBJS2 += EXPRS_PRINTER.o

	# PROG1 / 2 / 3 call yyparse (gen doesn't) (yet)
	OBJS123 += YY_PARSE.o 

	# PROG1 # e1 # uses
	OBJS1 += EXPRS.o

	# PROG2 # AFM # uses
	#
	# OBJS2 += sto_4k.o
	# OBJS2 += STO.o
	# OBJS2 += mmap_file.o

	# PROG3 ' JSON uses
	#

	# clean between phases # maybe

	CLEAN1 += $(from_lex_hh_1)
	CLEAN1 += $(from_yacc_hh_1)
	CLEAN1 += $(from_lex_cc_1)
	CLEAN1 += $(from_yacc_cc_1)

	CLEAN2 += $(from_lex_hh_2)
	CLEAN2 += $(from_yacc_hh_2)
	CLEAN2 += $(from_lex_cc_2)
	CLEAN2 += $(from_yacc_cc_2)

	CLEAN3 += $(from_lex_hh_3)
	CLEAN3 += $(from_yacc_hh_3)
	CLEAN3 += $(from_lex_cc_3)
	CLEAN3 += $(from_yacc_cc_3)

	CLEAN1 += $(OBJS1)
	CLEAN2 += $(OBJS2)
	CLEAN3 += $(OBJS3)
	CLEAN0 += $(OBJS0)

# END of ASSIGNMENT STYLE STATEMENTS
# START OF Dependency RULES

#all: test4
# all: test2
all: test0 test1 test2 test3

$(PROG0_exec): ../MK/Makefile
$(PROG1_exec): ../MK/Makefile
$(PROG2_exec): ../MK/Makefile
$(PROG3_exec): ../MK/Makefile

# SUFFIXES must be in TARGETS section
# you can include it, when you include ../MK/Makefile_SOMETHING

.SUFFIXES: .cxx .tab.c .tab.cc .cc .c .lex .y .o

# Generate YACC INPUT files (was from tcl)
# e1 # adding _psg _lex to see what_comes from where
#
#	gen_e1_yacc_2.y
#	gen_e1_lex_2.lex

# this is test1 - it generates the .lex .y files


# test1 is to run PSG_udef to produce filename .lex .y
#  gen_e1 is the name used in all generated files
#   PSG * e1 = new_PSG_for_math_C_expr(" e1 ");
#   print_GEN_for_flex_bison ...

#	# test1: $(into_yacc_y_1)

# DEPENDECY TREE should be here
# everything depends on  $(into_yacc_y_1)
# $(into_yacc_y_1) depends on  PROG0 ; run PROG0 with args

# lex follows yacc, because yacc produces the TABLE and TOKENS
# 
#	$(from_yacc_hh_1): $(into_yacc_y_1) 	# .y -> tab.hh
#	$(from_lex_hh_1): $(into_lex_lex_1) 	# .lex -> .hh # ??
#	$(into_lex_lex_1): $(from_yacc_hh_1)	# run BISON before FLEX

#	$(from_lex_cc_1): $(from_lex_hh_1) 	# edit .h triggers own recompile
#	$(from_lex_cc_1): $(into_yacc_y_1) 	# everything depends on Tree_top

$(from_yacc_hh_1): $(into_yacc_y_1)
$(from_yacc_hh_2): $(into_yacc_y_2)
$(from_yacc_hh_3): $(into_yacc_y_3)

# TOP does depend on a few things beyond PROG

$(into_yacc_y_0): ../src/EXPRS.h
$(into_yacc_y_1): ../src/$(G1).y_RULES 
$(into_yacc_y_2): ../src/$(G2).y_RULES 
$(into_yacc_y_3): ../src/$(G3).y_RULES 

# covered above
# $(PROG1_exec): ../src/$(G1).y_RULES 
# $(PROG2_exec): ../src/$(G2).y_RULES 
# $(PROG3_exec): ../src/$(G3).y_RULES 

#
# most nearby code want those tokens
# $(into_lex_lex_1): $(from_yacc_hh_1)

$(into_lex_lex_1): $(from_yacc_hh_1)
$(into_lex_lex_2): $(from_yacc_hh_2)
$(into_lex_lex_3): $(from_yacc_hh_3)

$(from_yacc_cc_1): $(from_yacc_hh_1)
$(from_yacc_cc_2): $(from_yacc_hh_2)
$(from_yacc_cc_3): $(from_yacc_hh_3)

$(from_lex_cc_1): $(from_yacc_hh_1)
$(from_lex_cc_2): $(from_yacc_hh_2)
$(from_lex_cc_3): $(from_yacc_hh_3)

$(from_lex_o_1): $(from_yacc_hh_1)
$(from_lex_o_2): $(from_yacc_hh_2)
$(from_lex_o_3): $(from_yacc_hh_3)


# fix
# EXPRS.o: $(from_yacc_hh_3)

unused34:
	# DEPENDENCY ... uses ...
	# $(from_lex_o_1)
	# $(from_yacc_hh_1)
	ls -lrt $(from_yacc_hh_1)
	#


$(into_yacc_y_1): $(PROG0_exec)
	#
	# GEN 1 $@
	# RUN $(PROG1_exec)
	# LEFT $(GEN_left)
	# GEN_ $(GEN_gen)
	# PSG $(G1)
	# adds _lex _yacc .lex .y 
	# later _lex _yacc .lex .y .cc .o _yac.tab.hh 
	# Generate .lex .y files # PSG and LEX # Tree C++ components #
	#
	./$(PROG0_exec) $(GEN_left) $(GEN_gen) $(G1) #
	#
	# Created
	# $(into_lex_lex_1)
	# $(into_yacc_y_1)
	#
	@# ls -l1t $(GEN_lex_1).lex  $(into_yacc_y_1)
	@ ls -l1t $(GEN_lex_1).lex  $(into_yacc_y_1)

$(into_yacc_y_2): $(PROG0_exec)
	#
	# GEN 2 $@
	# RUN $(PROG0_exec)
	# LEFT $(GEN_left)
	# GEN_ $(GEN_gen)
	# PSG $(G2)
	# adds _lex _yacc .lex .y 
	# later _lex _yacc .lex .y .cc .o _yac.tab.hh 
	# Generate .lex .y files # PSG and LEX # Tree C++ components #
	#
	./$(PROG0_exec) $(GEN_left) $(GEN_gen) $(G2) #
	#
	# Created
	# $(into_lex_lex_2)
	# $(into_yacc_y_2)
	#
	@# ls -l1t $(GEN_lex_2).lex  $(into_yacc_y_2)
	@ ls -l1t $(GEN_lex_2).lex  $(into_yacc_y_2)

$(into_yacc_y_3): $(PROG0_exec)
	#
	# GEN 3 $@
	# RUN $(PROG0_exec)
	# LEFT $(GEN_left)
	# GEN_ $(GEN_gen)
	# PSG $(GEN_psg_3)
	# adds _lex _yacc .lex .y 
	# later _lex _yacc .lex .y .cc .o _yac.tab.hh 
	# Generate .lex .y files # PSG and LEX # Tree C++ components #
	#
	./$(PROG0_exec) $(GEN_left) $(GEN_gen) $(G3) #
	#
	# Created
	# $(into_lex_lex_3)
	# $(into_yacc_y_3)
	#
	@# ls -l1t $(GEN_lex_3).lex  $(into_yacc_y_3)
	@ ls -l1t $(GEN_lex_3).lex  $(into_yacc_y_3)

# YACC OUTPUT files

# both .hh .cc produced at same time by bison
# but want to use $@ to tell bison $(from_yacc_cc_1)

# $(from_yacc_cc_1) $(from_yacc_hh_1):  $(into_yacc_y_1) 


$(from_yacc_hh_1): $(from_yacc_cc_1)
$(from_yacc_hh_2): $(from_yacc_cc_2)
$(from_yacc_hh_3): $(from_yacc_cc_3)

$(from_yacc_cc_1): $(into_yacc_y_1)
	#
	# BISON
	# FROM     < $<
	# CREATING @ $@
	#
	bison -d -o $@ $<
	#
	# CREATED @ $@
	#

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

# flex uses bisons tab.hh and the TOKEN lists there
# so make it dependent on the output from BISON from_yacc_hh_1 
# from_yacc_hh_1 GEN_yacc_1.tab.hh

$(from_lex_cc_1): $(into_lex_lex_1) $(from_yacc_hh_1)
	# GEN_lex_1 = $(GEN_lex_1)
	# expect  "gen_GEN_lex"
	#
	# FLEX
	# FROM     < $<
	# CREATING @ $@
	#
	flex -o $@ $(GEN_lex_1).lex
	#
	# CREATED @ $@
	#

$(from_lex_cc_2): $(into_lex_lex_2) $(from_yacc_hh_2)
	# GEN_lex_2 = $(GEN_lex_2)
	# expect  "gen_GEN_lex"
	#
	# FLEX
	# FROM     < $<
	# CREATING @ $@
	#
	flex -o $@ $(GEN_lex_2).lex
	#
	# CREATED @ $@
	#

$(from_lex_cc_3): $(into_lex_lex_3) $(from_yacc_hh_3)
	# GEN_lex_3 = $(GEN_lex_3)
	# expect  "gen_GEN_lex"
	#
	# FLEX
	# FROM     < $<
	# CREATING @ $@
	#
	flex -o $@ $(GEN_lex_3).lex
	#
	# CREATED @ $@
	#

# # CXX files
# 
# # all C files get all flags
# # when live both will be same ??
# # whan live may need loadable filter
# 
# .cc.o:                                                                         
# 	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  
# 
# .cxx.o:                                                                         
# 	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  
# 
# 

# $(OBJS0): $(HDRS)
$(OBJS1): $(HDRS)
$(OBJS2): $(HDRS)
$(OBJS3): $(HDRS)

prog0: $(PROG0_exec)

# everything depends on  $(into_yacc_y_1)
# test0: $(PROG0_exec)
test0: $(into_yacc_y_1)
test0: $(into_yacc_y_2)
test0: $(into_yacc_y_3)

test1: $(PROG1_exec)
	# test1
	./$(PROG1_exec) $T/test1.test

test2: $(PROG2_exec)
	# test2
	./$(PROG2_exec) $T/test2.test

test3: $(PROG3_exec)
	# test3
	./$(PROG3_exec) $T/test3.test

dgb_PROG1: $(PROG1_exec)
	# debug
	# BUT STDIN < $S/test1.test
	# errm
	# gdb --args ./$(PROG1_exec)
	#
	# -tui messes stderr with same tty
	gdb -tui --command ../.gdbinit_$(PROG1) --args ./$(PROG1_exec) $S/test1.test

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

dgb_PROG0: $(PROG0_exec)
	# debug
	gdb -tui --command ../.gdbinit_$(PROG0) --args ./$(PROG0_exec) $(GEN_left) $(GEN_gen) $(G2)

dgb_insight: $(PROG0_exec)
	# debug
	insight --args ./$(PROG0_exec) $(GEN_left) $(GEN_gen) $(G2)

$(PROG0_exec): $(PROG0).o  $(OBJS0) 
	# PROG0 - GEN -
	g++ $(PROG0).o $(OBJS0) $(LINKS) -o $@

$(PROG1_exec): $(PROG1).o $(OBJS1) 
	# PROG1 $(G1)
	g++ $(PROG1).o $(OBJS1) $(LINKS) -o $@

$(PROG2_exec): $(PROG2).o $(OBJS2) 
	# PROG2 $(G2)
	g++ $(PROG2).o $(OBJS2) $(LINKS) -o $@

$(PROG3_exec): $(PROG3).o $(OBJS3) 
	# PROG3 $(G3)
	g++ $(PROG3).o $(OBJS3) $(LINKS) -o $@

T4: $(PROG4_exec)
	# OK

CLEAN1: 
	# clean 1
	rm -f $(CLEAN1) 

clean: 
	# clean 
	rm -f $(CLEAN1) $(CLEAN2) $(CLEAN3) $(CLEAN0) 

clean_gen:
	# clean_gen
	# the generated files # list of
	rm -f $(gen_list)



