#	# SAMPLE easiest build with libs_apps

	# N is NAME of PROG1
	N=GCC_INIT

	CXX = g++
	OPTIM += -g
	INCLS += -I./

	OBJS += unused.o

# NO # $N.exec: $(OBJS)

# BUG #	Dur to .o.exec: $(OBJS) 

#	NOT NEEDED # just helps play

test: $N.exec
	# 
	# RUN ...
	#
	./$N.exec

$N.o:	$N.cxx
	g++ -g -O2 -c  $N.cxx

$N.exec: Makefile $N.o
	g++ -g -O2  -o $@ $N.o

clean:
	-rm -f $N.exec $(OBJS) $N.o
