2022-06-08 Jun Wed 18:24 # 50 #  README_MK.txt

	/home/gps/YEAR/src/PLAY/libs_apps/MK/./README_MK.txt
	ABS_OVER=/home/gps/YEAR/src/PLAY
	ABS_TOP=$(ABS_OVER)/libs_apps

	Some tools in the toolchain (gdb-like) need absolute pathnames
	You will need to edit ABS_OVER, or override it when invoked

 DIR LAYOUT

	ABS_TOP=$(ABS_OVER)/libs_apps

	TOP/MK/
	TOP/build_LGX_boxname/
	TOP/src/lib_base/buffer/blk1.cxx # and .h
	TOP/src/apps/main/play_code.cxx # main() is in each of these apps
	TOP/lib_pkgconfig/LGX/libs_apps_glib.pc # requires glib-2.0
	TOP/_run/mk_less_lots_k # run this to build target "lots"
	TOP/_run/mk # edit this to set vars for all build
	TOP/include/blk1.h --symb-link--> ../src/lib_base/buffer/blk1.h


	I use mc MidnightCommander as menu 
	I cd to _run
	I run ./mk_less_PICK_ONE

	It calls ./mk # via ./mk_less
	It cd's to ../build_LGX_boxname/

		if that does not exist you must create it
		interactive does not work well with mk_less
		so try ./mk first

		all of the build is built from that ../build_LGX_boxname/ dir
		all of the objs.o libs.so apps.exec go there

		"LGX" is for "Linux_Gnu_X11" 
		"W32" is not working at the moment, io issues
		"ARM" as in cross compiler is not working

			LGX on arm might work, Linux gcc

		All of these have their own dirs in lib_pkgconfig
		so that you can override for specific builds
		For now there is only LGX and "boxname" (uname -n)

	/libs_apps/MK/./README_MK.txt


2020-06-09 Jun Tue 18:11 README_MK.txts

 NODE = mkdir dirname_of_NODE()
 NODE = new_from_BENCH _is_a_dir_
 NODE = NODE_LOCN ../SUBDIR/. DONE_dirname
 DONE_RETVAL 
 LOCN_HERE {
 ...
 }

 NODE = subgroup_with-in_dir  LIST_of_PEER()
 NODE = new_from_BENCH _is_in_a_shared_dir_ // NAME = UDEF_as_filename
 NODE = NODE_LOCN ../SUBDIR/. DONE_dirname
 DONE_RETVAL 
 LOCN_HERE {
 ...
 }

	DIR_0 = ../MK/.
	DIR_0 += LOCN += EIGHT += BENCH_EIGHT
 	LIB_1 = new NODE=group("LIB_1") # base1
 	LIB_2 = new NODE=group("LIB_2") # base2 _ssl _gtk _tcl
 	LIB_3 = new NODE=group("LIB_3") # 

2020-06-09 Jun Tue 17:49
---------
... this has changed ... read it at your peril ... (not me ;-) ...
The makefiles are a thing of flux ...

TOP/MK/

	This is where the makefiles are
	./ == ../MK/. { TREE of USED CACHE } { SCRIPT == Makefile }
	../lib/pkgconfig/LGX/libs_apps.pc

TOP/MK/MK_HERE/

	MK_HERE is specific to this machine HERE
	MK_HERE is a symb link to the directory: MK_UNIX
	You could delete it and copy over MK_WIN32
	In MK_HERE, there is a makefile stub for each linked library
	This makes it easier to edit one for your system,
	and upgrade to newer releases

	mkdir /tools/CTXT/libs/ // aka tools_2000 // 
	but that varies

TOP/MK/VAR_OBJS_xxxx

	S(OBJS_xxx) is a variable listing lots of file.o
	VAR_OBJS_xxx is a makefile, where the variable is set

	If you add a file.o you will need to add it to the list.
	Try to find the right list for the right thing,
	as different lists go into different libraries

	NB every filename.o must be unique! (at least within libs_apps)
	because the corresponding source files are found with VPATH.
	The idea is for me to have a set of non-conflicting files,
	and has worked well so far.

WITH_PDFLIB=1

	This will link the library with -Ixxx -lpdflib ...

	NB The libraries will need WITH_XX=1 correctly set. (seems OK)
	Modularity is possible, but not every variation + combination.

WITH_SPOUT=1

	This compiles the spout module (always on, untested off)
	That generates PS cans calls ps_to_pdf

	This project left here as a placeholder, to return from
	${HOME}/YEAR/src/libs_apps/MK
	${HOME}/YEAR/src_build/YEAR_src_PLAY/GEN_/GEN_SPOUT/MK

	Then you can write print_page

		tree_to_paper // data_to_screen
		available_paper_pen_ink 
		 draw_commands
		  

TOP/lib/pkgconfig

	there is a new pgc-config for clients
	it contains hardcoded paths (similar to those in MK_HERE)
	and is good for clients. The libraries still need make

	NB doing that exposes where make is better than pkg-config,
	but also where pkg-config reduces duplication (but does it
	resolve the different -rpath syntax's for gcc/ld? - it cant
	because it doesnt invoke gcc, it just provides a string!



# Mon Aug 24 22:03:07 BST 2009
#
# libbase2.so
#
# 
 #	DLL11=libbase2.so
 # etc	

/nfs/md7/GPS/ftp_2009/ftp_build_2009/
	make-3.81/doc
		make.info
		make.info-1
		make.info-2
		10.5 Defining and Redefining Pattern Rules
%.dll:
	text of rule
	expands % in text



