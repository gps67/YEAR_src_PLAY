N=read1

test2: $N.exec 
	-#./$N.exec remix.out
	./remix

test: $N.exec 
	./$N.exec

$N.exec: $N.cxx
	g++ -g -o $@ $<



