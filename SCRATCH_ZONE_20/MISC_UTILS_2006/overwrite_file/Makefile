	N=overwrite
	P=$N.exec
	F=somefile.tmp

	R=-Wl,--rpath=/tools/2005/ccs4/lib

test: $P
	echo GOODBYE > $F
	./$P $F

$P: $N.cc
	g++ -g $R -o $@  $<
