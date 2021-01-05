#!/usr/bin/false

	NOW THERE ARE DEPENDENCIES ON g_print AND THINGS
#!/usr/bin/make -f

	INCL  += `pkg-config --cflags glib-2.0`
	LIBS  += `pkg-config --libs glib-2.0`
	OPTIM += -O2 -g

	OBJS1 += stats_sysinfo.o 
	OBJS1 += stats_sysinfo_main.o

default: stats_sysinfo

.SUFFIXES: .cxx .c .o

.c.o:
	gcc $(OPTIM) $(INCLS) $(DEFS) -c $<

.cxx.o:
	g++ $(OPTIM) $(INCLS) $(DEFS) -c $<

stats_sysinfo: $(OBJS1)
	g++ -$(OPTIM) -o $@ $<
	strip "$@"
	./$@
