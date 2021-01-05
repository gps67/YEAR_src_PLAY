INCL  += `pkg-config --cflags glib-2.0`
LIBS  += `pkg-config --libs glib-2.0`

call_sysinfo.exec: call_sysinfo.cxx
	g++ -O2 -g -o $@ $(INCL) $< $(LIBS)
	strip "$@"
	# 
	# PRESS CTRL-C and make deletes the .exe
	#
	./$@
