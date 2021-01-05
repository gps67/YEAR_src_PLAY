INCL  += `pkg-config --cflags glib-2.0`
LIBS  += `pkg-config --libs glib-2.0`

proc_loadavg.exec: proc_loadavg.cxx
	g++ -O2 -g -o $@ $(INCL) $< $(LIBS)
	strip "$@"
