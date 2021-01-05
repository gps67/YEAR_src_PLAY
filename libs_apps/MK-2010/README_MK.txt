... this has changed ... read it at your peril ... (not me ;-) ...
The makefiles are a thing of flux ...

TOP/MK/

	This is where the makefiles are

TOP/MK/MK_HERE/

	MK_HERE is specific to this machine HERE
	MK_HERE is a symb link to the directory: MK_UNIX
	You could delete it and copy over MK_WIN32
	In MK_HERE, there is a makefile stub for each linked library
	This makes it easier to edit one for your system,
	and upgrade to newer releases

	Personally I have most things in /tools/2000/libs/
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



