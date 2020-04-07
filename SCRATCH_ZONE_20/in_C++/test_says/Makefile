N=test_says

test1: $N.exec
	./$N.exec

$N.exec: $N.cxx
	g++ -g -o $@ $<
