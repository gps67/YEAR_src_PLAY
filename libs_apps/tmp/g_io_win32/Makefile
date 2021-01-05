N=sock_cli_test1

Xgpp = /opt/IMCROSS/i386-mingw32/bin/g++

	WIN32_LIBS += -lgdi32
	WIN32_LIBS += -lwsock32
#	WIN32_LIBS += -lwindows	# no console and needs win_main()
	# 
	# to compile Gtk apps on WIN32 you must set the env var
	# export PKG_CONFIG_PATH=/opt/IMCROSS/i386-mingw32/lib/pkgconfig
	#
	COMP_GTK += `pkg-config --cflags gtk+-2.0`
	LINK_GTK += `pkg-config --libs gtk+-2.0`

test32: $N.exe
	wine $N.exe

test: $N
	./$N

$N: $N.c
	g++ -g -o $@ $< $(COMP_GTK) $(LINK_GTK)

$N.exe: $N.c
	$(Xgpp) -g -o $@ $<  $(COMP_GTK) $(WIN32_LIBS) $(LINK_GTK)
