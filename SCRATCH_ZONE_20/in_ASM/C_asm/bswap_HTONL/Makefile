N=bswap_htonl

test1: $N.exec
	./$N.exec ; perror

$N.s: $N.cxx Makefile ASM_byte_swap.h
	g++ -o $@ -S -g -O2 $<
	cat $@

$N.o: $N.s
	g++ -c -o $@ -g -O2 $<
	objdump -dsS $@

$N.exec: $N.o
	g++ -o $@ -g -O2 $<

clean:
	rm -f $N.exec $N.s $N.o
