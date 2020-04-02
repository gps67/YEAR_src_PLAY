
E=.exec
F=test_align
test: $F.exec
	./$F.exec

$F.exec: $F.cxx
	g++ -o $F.s -S -g $<
	g++ -o $@ -g $<
