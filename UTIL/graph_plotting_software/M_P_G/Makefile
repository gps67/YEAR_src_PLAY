N=read1

test2: $N.exec 
	./$N.exec mpg.txt | tee mpg_table.txt
	./mpg_plot_1
	-# ./$N.exec remix.out
	-# ./remix

test: $N.exec 
	./$N.exec

$N.exec: $N.cxx
	g++ -g -o $@ $<



