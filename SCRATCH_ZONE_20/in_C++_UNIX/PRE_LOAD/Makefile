
test1: MAIN_one.exec PRE_LOAD_one.so

	LD_PRELOAD=./PRE_LOAD_one.so ./MAIN_one.exec

MAIN_one.exec: MAIN_one.c

	gcc -o $@ $<

PRE_LOAD_one.so: PRE_LOAD_one.o

	ld -shared -o $@ -fPIC $< -ldl

PRE_LOAD_one.o: PRE_LOAD_one.c

	gcc -c -o $@ -fPIC $<
