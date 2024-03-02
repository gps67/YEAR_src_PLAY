N=bswap_htonl

	include ./MK/MK_with_libs_apps
	PKGS += libs_apps_base2



test1: $N.exec
#	./$N.exec ; perror

$N.o: $N.cxx Makefile ASM_byte_swap.h
	g++ -o $@ -S -g -O2 $<

$N.s: $N.o
	objdump -dsS $N.o
	cat $@

$N.exec: $N.o
	g++ -o $@ -g -O2 $<

clean:
	rm -f $N.exec $N.s $N.o
