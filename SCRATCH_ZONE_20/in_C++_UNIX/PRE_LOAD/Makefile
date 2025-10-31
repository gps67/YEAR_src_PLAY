
CFLAGS += -pedantic
OBJS += MAIN_one.exec
# OBJS += MAIN_one.o
OBJS += PRE_LOAD_one.so
OBJS += PRE_LOAD_one.o


test1: MAIN_one.exec PRE_LOAD_one.so

	#
	# see L_x_R
	# see M_x_N
	# see   z_N // GROW "{ idx = N ++ }" IDX
	#
	# all you have to do it SETENV 
	# LD_PRELOAD=./PRE_LOAD_one.so
	#
	# then call the original APP
	# ./MAIN_one.exec
	#
	# and the "open" function gets hijacked to run UDEF code
	#
	LD_PRELOAD=./PRE_LOAD_one.so ./MAIN_one.exec

MAIN_one.exec: MAIN_one.c

	gcc -o $@ $<

PRE_LOAD_one.so: PRE_LOAD_one.o

	ld -shared -o $@ -fPIC $< -ldl

PRE_LOAD_one.o: PRE_LOAD_one.c

	gcc -pedantic -c -o $@ -fPIC $<

clean:
	rm -f $(OBJS)
