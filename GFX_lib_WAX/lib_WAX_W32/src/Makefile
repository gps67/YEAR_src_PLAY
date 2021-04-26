
G=/usr/bin/i686-w64-mingw32-g++ -mwindows
# -mwindows

# /usr/lib/gcc/i686-w64-mingw32/7.3-win32/libgcc_s_sjlj-1.dll



OBJS += hello.o
# OBJS += wnd_class.o

runit: hello.exe
	wine hello.exe

hello.exe: $(OBJS) Makefile
	$G -g -o $@ $<

clean:
	rm hello.exe hello.o

.SUFFIXES: .cxx .c .o .a .so .exe

.cxx.o:
	$G -g -c -o $@ $<
