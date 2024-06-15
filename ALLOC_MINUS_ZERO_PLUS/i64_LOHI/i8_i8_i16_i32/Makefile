
E=.exec
F=test_align

GCC=gcc

# GCC=tcc -xc 
# tcc does not work because it cant handle "int & areg"

# GCC=gcc

test: $F.exec
	./$F.exec

$F.exec: $F.cxx
	# $(GCC) -o $F.s -S -g $<
	$(GCC) -o $@ -g $<

clean:
	rm -f $F.exec *.o
