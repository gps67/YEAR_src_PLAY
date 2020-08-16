INCL  += `pkg-config --cflags glib-2.0`
LIBS  += `pkg-config --libs glib-2.0`

test: proc_loadavg.exec
	#
	#
	# CTRL-C keeps .exec - not a direct temp target of unfulfilled clause
	#
	#
	./$<

proc_loadavg.exec: proc_loadavg.cxx
	g++ -O2 -g -o $@ $(INCL) $< $(LIBS)
	strip "$@"
