#!/usr/bin/env make

	CXX = g++
	OPTIM = -g
	INCLS = -I../
#	INCLS = -I../buffer

	dot_EXE=.exec

	# add any -D defines
	DEFS = 

#	# add any options to ld
#	LINKS = -L/usr/X11R6/lib -lX11
#	LINKS = -L/tools/2000/X11R6/lib -lX11

	S=../src
	VPATH +=$S
#	VPATH +=$S/buffer # more places where src files are
#	eg to merge in 5 files from another project
#	ln -s there nearhere
#	VPATH += $S/nearhere
#	INCLS += $S/nearhere/include
#	OBJS += file5.o
#	that will leave their source there,
#	and compile them here, link in to PROG

	# common VARS and use already compiled libs_apps
	include ../MK/MK_with_libs_apps # OK - ish

	PROG1=test1
	PROG2=bitfield_order

	EXEC1=$(PROG1)$(dot_EXE)
	EXEC2=$(PROG2)$(dot_EXE)

all: run_test1

ifeq (0,1)
# include MK_SUFFIXES from MK_ext_lib
.SUFFIXES: .cxx .c .o                                                           
.cxx.o:                                                                         
	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  
endif

$(EXEC1): $(OBJS)
$(EXEC2): $(OBJS)

$(OBJS): $(HDRS)

run_test1: $(EXEC1)
	./$(EXEC1)

run_test2: $(EXEC2)
	./$(EXEC2) # args

ifeq (1,0)
# we no-longer link it - the MK_SUFFIXES .o.exec does
$(EXEC1): $(OBJS) $(PROG1).o
	g++ $(PROG1).o $(OBJS) $(LINKS) -o $@

$(EXEC2): $(OBJS) $(PROG2).o
	g++ $(PROG2).o $(OBJS) $(LINKS) -o $@
endif

clean: 
	rm -f *.o $(EXEC1)  $(EXEC2)  test2 fa
