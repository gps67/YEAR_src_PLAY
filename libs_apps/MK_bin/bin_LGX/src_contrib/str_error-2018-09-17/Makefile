N=str_error

test: $N.exe
	- ./$N.exe 4 /DEV/THING
	- ./$N.exe 6 /DEV/THING
	./$N.exe 0 /DEV/THING
	cp $N.exe perror

$N.exe: $N.c
	gcc -o $@ -O3 $<
