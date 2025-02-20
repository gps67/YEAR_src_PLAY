
CFLAGS += -pedantic
OBJS += MAIN_one.exec
# OBJS += MAIN_one.o
OBJS += PRE_LOAD_one.so
OBJS += PRE_LOAD_one.o


test1: MAIN_one.exec 
	./MAIN_one.exec

test2: MAIN_one.exec PRE_LOAD_one.so

	LD_PRELOAD=./PRE_LOAD_one.so ./MAIN_one.exec

MAIN_one.exec: MAIN_one.c

	gcc -o $@ $<

PRE_LOAD_one.so: PRE_LOAD_one.o

	ld -shared -o $@ -fPIC $< -ldl

PRE_LOAD_one.o: PRE_LOAD_one.c

	gcc -pedantic -c -o $@ -fPIC $<

clean:
	rm -f $(OBJS)
