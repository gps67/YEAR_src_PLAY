#!/usr/bin/env make

PROGNAME = PATH_TOOL

test: $(PROGNAME)
	./test1
	./test2
	./test3

# CXX=g++ # linux
CXX=CC # sxde # gmake
FLAGS_COMPILE += -g
FLAGS_COMPILE += -O2

$(PROGNAME).o: $(PROGNAME).cxx Makefile
	$(CXX) $(FLAGS_COMPILE) -c -o $@ $<

$(PROGNAME): $(PROGNAME).o
	$(CXX) $(FLAGS_COMPILE) -o $@ $<
	strip $@
	cp -p $(PROGNAME) ..

clean:
	rm -f $(PROGNAME) $(PROGNAME).o

