	N=vtbl_math

	CXX = g++
	OPTIM += -g
	INCLS += -I./
	PKGS += libs_apps_base1
	include MK_with_libs_apps

	PROG1=$N
	# OBJS1 += $(PROG1).o

test: $N.exec
	# 
	# RUN ...
	#
	./$N.exec

clean:
	-rm -f $N.exec $(OBJS) $N.o

#$N.exec: $(PROG1).o $(OBJS1)
#	g++ $(PROG1).o $(OPTIM) $(INCLS)  $(OBJS1) $(LINKS) $(LIBS) -o $@

# .SUFFIXES: .cxx .tab.c .tab.cc .cc .c .y .o                                                           
#.cc.o:                                                                         
#	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  
#
#.cxx.o:
#	$(CXX) $(OPTIM) $(INCLS) $(DEFS) -c $<                                  
