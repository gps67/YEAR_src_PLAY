#	# SAMPLE easiest build with libs_apps

	# N is NAME of PROG1
	N=vtbl_math

	CXX = g++
	OPTIM += -g
	INCLS += -I./

	PKGS += libs_apps_base1
	include MK_with_libs_apps

#	NOT NEEDED just target $N.exec
#
#	PROG1=$N
#	OBJS1 += $(PROG1).o

# not in the SUFFIX rule as a prerequisite
# we have to either remove it or add dependency
	OBJS += unused.o

$N.exec: $(OBJS)

# BUG #	Dur to .o.exec: $(OBJS) 

#	NOT NEEDED # just helps play
$N.exec: Makefile

test: $N.exec
	# 
	# RUN ...
	#
	./$N.exec

clean:
	-rm -f $N.exec $(OBJS) $N.o
