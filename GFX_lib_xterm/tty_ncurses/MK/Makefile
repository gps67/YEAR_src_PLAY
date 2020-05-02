#!/usr/bin/env make

	CXX = g++
	OPTIM += -g
	INCLS += -I./
	INCLS += -I../
#	INCLS += -I/tools/2000/X11R6/include

	# this should be from VPATH
	# IT IS # INCLS += -I../src

	DEFS = 
#	LINKS = -L/usr/X11R6/lib -lX11
	LINKS = -lncurses

	# S=../src
	S=..
	VPATH +=$S
	VPATH +=$S/src
	VPATH +=$S/src/PSG_Tree

# I dont know - this is supposed to be either
	VPATH +=$S/../../STO_BYTES
	INCLS +=-I $S/../../STO_BYTES # OK

	include ../MK/Makefile_libs_apps

	PROG1=test_1
	PROG1_exec=$(PROG1).exec

	PROG2=test_2
	PROG2_exec=$(PROG2).exec

	PROG3=test_3
	PROG3_exec=$(PROG3).exec

# OTHER OPTION: 
#	OBJS1 += $(PROG1).o
#	OBJS2 += $(PROG2).o
#	OBJS2 += $(OBJ1)
#	OBJS2 = & OBJS1 # HMMM syntax of CT implement B within C bind_var
#	DIALECT EA_EXPR OBJS_LIST_1 = POOL of filename.o or OTHER
#	DIALECT EA_EXPR OBJS_LIST_2 = POOL of filename.o or OTHER
# 	LEXICON EA_EXPR ...
#	Makefile
#	LATER CLEAN += OBJS # so dont add here, add there ...
	CLEAN1 += $(PROG1).o
	CLEAN2 += $(PROG2).o
	CLEAN3 += $(PROG3).o

	CLEAN1 += $(PROG1_exec)
	CLEAN2 += $(PROG2_exec)
	CLEAN3 += $(PROG3_exec)


	OBJS1 += tty_curses.o
	OBJS1 += tty_curses_WIN.o
	OBJS1 += tty_curses_WIN_BOX.o
	OBJS1 += tty_curses_CSR.o
	OBJS1 += tty_curses_UDLR.o
# above are this library module
# I foresee typedef screen iNCURSES_screen; // CURSES_SCREEN is the VTBL base 
# SCREEN.win is the ncurses_screen
# It wraps all the standard move( Y, X )
# it doesn't have to be VTBL either, if no other fields, use original PTR_VAL
# TODO also add chattr
# TODO tty_curses_CSR does not itself keep a csr, but it does direct access, via
# CSR.get_XY( RET_t & RET_REF ) // HOW ?? return VAR to non PTR, conv * to &
# HOW = outer function provides a local VAR_REF ref_to_var; passes into &
# YES that is actually two different '&' The container pointer, the ITEM, in 1
# SO KDB shows two things crossing over into the same implementation crystal
# below is modules extra library, tests, variations, demo, extras

	GDB_CMD += gdb
	GDB_CMD += -tui

	OBJS1 += argv_decoder.o

	OBJS1 += test_SHAPE_BASE.o
	OBJS1 += test_SHAPE_ONE.o
	OBJS1 += test_SHAPE_TWO.o

	# OBJS1 order matters - maybe put into .a to mix it up
	# put top last ? or only matters when .a ?

	OBJS1 += test_main_test_1.o
	OBJS2 += test_main_test_2.o	# why different exe? decode ARGS
	OBJS1 += test_main_test_3.o	# why different exe? decode ARGS

	# order of this paragraph in makefile, disregarded, but here

	CLEAN1 += $(OBJS1)
	CLEAN2 += $(OBJS2)
	CLEAN3 += $(OBJS3)

	CLEANS += $(CLEAN1) $(CLEAN2) $(CLEAN3)

default: test3


all: test3 test1 test2

build: $(PROG1_exec) $(PROG2_exec) $(PROG3_exec) 

# $(PROG2_exec)


.SUFFIXES: .cxx .tab.c .tab.cc .cc .c .y .o

.cc.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  

.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  


$(OBJS1): $(HDRS)
$(OBJS2): $(HDRS)

#test1: $(PROG1_exec)
#	./$(PROG1_exec) gen

test1: $(PROG1_exec)
	./$(PROG1_exec)

test2: $(PROG2_exec)
	./$(PROG2_exec)

test3: $(PROG3_exec)
	# test3
	pwd
	# curses from STDIN not a tty
	./$(PROG3_exec) # < $S/test3.test

# testnn : $(PROG4_test) 
#	./$(PROG4_test) testnn

dgb: dgb_3
	## to select a different dbg edit Makefile ##

dgb_1: $(PROG1_exec)
	# debug
	$(GDB_CMD) --args ./$(PROG1_exec) # ARGS # 
#	$(GDB_CMD) --args ./$(PROG2_exec) < $S/test2.test
#	$(GDB_CMD) --args ./$(PROG3_exec) < $S/test3.test

dgb_3: $(PROG3_exec)
	# debug
#	$(GDB_CMD) --args ./$(PROG1_exec) # ARGS # 
#	$(GDB_CMD) --args ./$(PROG2_exec) < $S/test2.test
	$(GDB_CMD) --command ../.gdbinit_$(PROG3_exec) --args ./$(PROG3_exec) # < $S/test3.test

$(PROG1_exec): $(PROG1).o $(OBJS1) 
	# g++ $< $(LINKS) -o $@
	g++ $(PROG1).o $(OBJS1) $(LINKS) -o $@

$(PROG2_exec): $(PROG2).o $(OBJS2) 
	# g++ $< $(LINKS) -o $@
	g++ $(PROG2).o $(OBJS1) $(OBJS2) $(LINKS) -o $@

$(PROG3_exec): $(PROG3).o $(OBJS3) $(OBJS1)
	# g++ $< $(LINKS) -o $@
	g++ $(PROG3).o $(OBJS1) $(OBJS3) $(LINKS) -o $@

clean: 
	# rm -f $(CLEAN12) $(CLEAN23) 
	rm -f $(CLEANS)


# I need to check those different spellings in the comments
# But easier to wait until EXPR_Makefile brings its LEXICON $< means ...

# PROG1 generated FILE2
# FILE2 generated RESULT3
# now tidy up, each activity by its own list, generated in the way from 1 to 2
# CLEAN12 
# now tidy up, each activity by its own list, generated in the way from 2 to 3
# CLEAN23 
# Maybe RESULT is still there ?

# RENAME test libr extn
# need a Makefile,
# for people to grow their own libr_extn_item
# for people to grow their own libr_extn
# for people to grow their own libr_extn_tests // layers, zone, personal, test
# for someone to integrate into name allocated LIBR //namespace + a few top ids
