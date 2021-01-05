
	call set_env.bat
	cd ..\_obj_win32
	rem make  WIN32=1 -f ../MK/Makefile %1 %2 %3 %4 
	mingw32-make  WIN32=1 -f ../MK/Makefile %1 %2 %3 %4 
	rem > fa.tx
	pause Press any key to continue
