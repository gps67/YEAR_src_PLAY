N=u32_hilo_test

INCLS += ASM_hilo.h

FLAGS += -g
FLAGS += -O2

# FLAGS += -m32 # -D __i386__

default: test1

clean:
	rm $N.o $N.s $N.exec


default: test1

clean:
	rm $N.o $N.s $N.exec

test1: $N.exec
	./$N.exec ; perror $?

$N.s: $N.cxx Makefile $(INCLS)
	g++ -o $@ -S $(FLAGS) $<
	cat $@

$N.o: $N.s
	g++ -c -o $@ $(FLAGS) $<
	objdump -dsS $@

$N.exec: $N.o
	g++ -o $@ $(FLAGS) $<
