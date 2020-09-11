#!/usr/bin/env make

# chroot and suid back to callers id # run pretty much anything
PROGONE = chrt
# suid and run ./M pretty much anywhere!
PROGTWO = suid_M
EXECONE=$(PROGONE).exec
EXECTWO=$(PROGTWO).exec


FLAGS_COMPILE += -g
FLAGS_COMPILE += -O2
# FLAGS_COMPILE += -m32 # this messes up all includes when not there
# NO # FLAGS_COMPILE += -nostdlib # exceptions need a basic C++ lib

test: $(EXECONE)
	./test1
	./test2
	./test3

$(EXECTWO): $(PROGTWO).o
	gcc $(FLAGS_COMPILE) -o $@ $<
	strip $@
	# chgrp 100 $@
	chown root $@
	chmod 4755 $@
	ls -ld $@

$(PROGTWO).o: $(PROGTWO).c Makefile
	gcc $(FLAGS_COMPILE) -c -o $@ $<

$(PROGONE).o: $(PROGONE).c Makefile
	gcc $(FLAGS_COMPILE) -c -o $@ $<

$(EXECONE): $(PROGONE).o
	gcc $(FLAGS_COMPILE) -o $@ $<
	strip $@
	# chgrp 100 $@
	chown root $@
	chmod 4755 $@
	ls -ld $@
	cp -p $(EXECONE) ..
	cp -p $(EXECONE) ../../bin/

../$(PROGONE)_static: $(PROGONE).o
	gcc $(FLAGS_COMPILE) -static -o $@ $<
	strip $@

clean:
	rm -f $(EXECONE) $(PROGONE).o
	rm -f $(EXECTWO) $(PROGTWO).o

