	N=vtbl_math

	CXX = g++
	OPTIM += -g
	INCLS += -I./
	PKGS += libs_apps_base1
	include MK_with_libs_apps

#	PROG1=$N
	# OBJS1 += $(PROG1).o

test: $N.exec
	# 
	# RUN ...
	#
	./$N.exec

clean:
	-rm -f $N.exec $(OBJS) $N.o
