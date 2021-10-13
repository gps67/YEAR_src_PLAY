(
	PFX=$HOME/G/prefix
	export PKG_CONFIG_PATH=$PFX/lib/pkgconfig

	INCLS=` pkg-config mozjs-93 --cflags `

	INCLS="-isystem $PFX/include/mozjs-93"

	set -x
	g++ -g $INCLS -c SM_BASICS.cxx
) 2>&1 | less

# ++ g++ -g -isystem /home/gps/G/prefix/include/mozjs-93 -c SM_BASICS.cxx

