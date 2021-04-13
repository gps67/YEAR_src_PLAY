# needs cd then call
#!/usr/bin/make -f
# this SHOULD be run in $(ABS_TOP)/_obj by the calling script

#	TIGHT_GRIP_can_be_relaxed except Makefile notices SPACES
# TODO	MODULE get EIGHT _Module_Names Sub_Modules[ idx_u8 u3 ]
# TODO	MODULE gui EIGHT _Module_Names STR_t Name[ idx_u8 u3 ]

	LIB11=base
#	LIB12=base_glib # HARPOONED for now, stuck inside base[1]

	LIB21=base2
	LIB22=base2_ssl
	LIB23=base2_gtk
# TODO 	LIB24=base2_LIB3 LIBS_accts_gtk %{LIB3} "%s_sar_plot_post" LIBS3 ...
# it is how encoder/decoder N_alloc BitField_Layout gives SPACE for EIGHT PLUS
# plus is any upto u32 # PAGE_alloc_256

	LIB33=gtk_text
	LIB34=s_apps_media
	LIB35=accts_gtk
	LIB36=sar_plot_post

	# libs_apps_media.so

# _run/mk # caller script MUST set these VARS before calling make
# M7 is `uname -n`
# TGT is one of { LGX, WIN32, i586, ARM5, ARM9 } # except now only LGX

	# ABS_OVER	/home/gps/YEAR/src
	# ABS_TOP	${ABS_OVER}/libs_apps
	# TGT		LGX
	# OBJ_dirname	build_LGX_M7
	# ABS_OBJDIR	${ABS_TOP}/${OBJ_dirname}
	# REL_OBJDIR	../${OBJ_dirname}

	# WITH_PYTHON	# undefined or not [ -z "$WITH_PYTHON" ]

# mk => Makefile => pkg-config

	# you must also setup a dir for each ${TGT}
	# ../lib_pkgconfig/${TGT}
	# and it will override the pkg-config.pc files in its ..

######### ???????? ########
#
# PKGS - problem is multiple targets of different levels
# eg base2 needs things that base1 does not
# so dont add base2 PKGS to all
# but maybe satisfy ALL and over provide
# but that leaves base1 exe's loading librs they dont need ?
# maybe into_base was about that, add it to the _into_base1.pc
# and not here, noticed for WITH_PYTHON at core obj level
#
# I am not sure how to section off area that only use subsets
# ie when linking base, do no include self in obj list
# which is what _uses_base does
# so removing and lookinf for where to add it
#
# want rule per stanza PKGS += PKGS2 # then run pkg-config
# or move back to GTK_INCL $(GTK_LINK) $(LINK_with_base2)
# tricky using PKGS += libs_apps_base2 in outer scripts

#	# -I is all that most things need? and rpath
#	PKGS += libs_apps_base_own_paths
	PKGS += _into_base1
#
#	PKGS += glib-2.0
#	PKGS += libs_apps_uses_libz
#	PKGS += libs_apps_base_uses_list
#	 PKGS += libs_apps_base2_uses_list
#	PKGS += libs_apps_base2
#	CANT link something against itself

# on WIN32 set this in the calling maker.bat file, or edit this file

#	WIN32=0 # see Makefile_X_WIN32
#	still used outside to not select a list of objects to build

#	# SEE TGT={WIN32,LGX,...} as it now does that job via XGCC

# on twin UNIX - different compilers! # UNUSED?

	UNAME=$(shell uname -n)
	UNAME_m=$(shell uname -m) # x86_64

# ObjectType multi-build environments (on same machine) # UNUSED?

	OT=cc
	OT=py
	OT=js

#
# SOURCE + OBJECT TREE TOPS
#

# sorry about this - full length paths here, in mk_x.sh and in pkg-config files

	#
	# the directory (ABOVE) where this tree is housed
	#
	#	/media/X3/2014/src/
	#
	# ABS_OVER is set in the calling mk file which overrides everything
# include /home/gps/YEAR/src/libs_apps/MK/Makefile_SITE_ABS_OVER
# include ../MK/Makefile_SITE_ABS_OVER

ifeq (1,0)
	# makefile should ingnore this line because already det as ENV var
	ABS_OVER=/home/gps/YEAR/src/PLAY/libs_apps/

	# TODO check when set
	# ABS_OVER=/media/X3/2011/src/

	# the directory name of this project is fixed - SYMB LINK
	#
	#	libs_apps
	#
	ABS_TOP=$(ABS_OVER)/libs_apps


	# You must run this from the $ABS_TOP/_run/ (level) directory 
	#
	# from $(ABS_OVER)/libs_apps/libs_apps_obj_$(TGT)
	REL_TOP=..
	REL_OVER=../..

	OBJ_dirname=libs_apps_obj_$(TGT)_$(UNAME) # or build_...
endif

	#
	# use TGT in pkg-config files
	#
	PKG_CONFIG=pkg-config --define-variable=TGT=$(TGT)

	#
	# these two pairs are the same
	# note how $TGT overrides the default
	#
	export PKG_CONFIG_LIST
	PKG_CONFIG_LIST += .
	PKG_CONFIG_LIST += ../pkgconfig/$(TGT)/$(UNAME)
	PKG_CONFIG_LIST += ../pkgconfig/$(TGT)
	PKG_CONFIG_LIST += ../pkgconfig
	PKG_CONFIG_LIST += $(ABS_TOP)/pkgconfig/$(TGT)/$(UNAME)
	PKG_CONFIG_LIST += $(ABS_TOP)/pkgconfig/$(TGT)
	PKG_CONFIG_LIST += $(ABS_TOP)/pkgconfig
	PKG_CONFIG_LIST += $(HOME)/G/prefix/lib/pkgconfig
	PKG_CONFIG_LIST += $(HOME)/G/mozjs/lib/pkgconfig
#	PKG_CONFIG_LIST += /tools/CTXT/py/lib/pkgconfig
#	PKG_CONFIG_LIST += /tools/CTXT/py/lib/pkgconfig/falsely

# now here
ifeq ($(WITH_PYTHON),1)
	# this is being used
	# DEFS += -DWITH_PYTHON_PLUS
	# OK PKGS not being used - am using fixed lists
	DEFS += -DWITH_PYTHON
	PKGS += python3-embed
	PKG_CONFIG_LIST += /tools/CTXT/py/lib/pkgconfig
endif

	# git log -1 --format=%ad --date=local
	# shell
	GIT_DATE=$(shell git log -1 --format=%ad --date=local)
	DEFS += -DGIT_DATE='"$(GIT_DATE)"'
##	fprintf(stderr, "GIT_DATE %s\n", GIT_DATE ); // -DGIT_DATE='"..."'


	# PKG_CONFIG_LIST += /usr/lib/pkgconfig last
	#
	# also added to by Makefile_X_$(TGT)
	#

	# binaries are not installed by this makefile
	#
	#	$(OVER)/libs_apps/libs_apps_obj_WIN32
	#	$(OVER)/libs_apps/libs_apps_obj_i586
	#	$(OVER)/libs_apps/libs_apps_obj_LGX
	# and this is set in "mk"

	# new from here
	ABS_LIB_TGT=$(ABS_TOP)/$(OBJ_dirname)
	REL_LIB_TGT=$(REL_TOP)/$(OBJ_dirname)


######### RELIC #### an external tree of files to link against
##	# The calling script (or adjacent Makefile_X_install )
##	#
##	#
##	ABS_FROM_OVER=/tmp/ABS_OVER_FROM/
##	ABS_FROM_TGT=$(ABS_FROM_OVER)/FROM_ftp_build_$(TGT)
##	ABS_FROM_ROOT=$(ABS_FROM_TGT)/ROOT_libs_apps
##	ABS_TGT_ROOT=$(ABS_FROM_ROOT)
##	ABS_ROOT_TGT=$(ABS_FROM_ROOT)

#
# REL_ or ABS_
#
	# OVER is the choice of $(ABS_OVER) or $(REL_OVER)
	# ditto TOP

	# I dont need absolute paths
	# cooledit requires an absolute path, for jump-to-error
	# -or- relative to _run_/_build_ dir
	# sorry, that means you have to edit it

	# either-way, you must create simple-name links, such as:
	# libs_apps -> libs_apps-2005-01-31
	# mm3_scan -> mm3_scan-2004-04-30
	#

	# other projects include this makefile
	# which includes other makefiles
	# which need to find ABS_TOP
	# so make a rel symb-link, or abs

	# OVER=../..
	# ABS_TOP=..

ifeq (1,1)
	# ABS
	OVER=$(ABS_OVER)
	TOP=$(ABS_TOP)
else
	# REL
	OVER=$(REL_OVER)
	TOP=$(REL_TOP)
endif

	# rpath preferes absolute paths ...

	M=$(ABS_TOP)/MK#		# where the Makefiles are
	S=$(ABS_TOP)/src#		# where the source dirs are
	O=$(ABS_TOP)/$(OBJ_dirname)#	# there the objects are (if not .)
	ABS_O=$(ABS_TOP)/$(OBJ_dirname)# for rpath on devel system

#
# WITH_PACKAGES
#

	#
	# its probably hard work to compile without SSL # move it to base2
	#

		WITH_SSL=1
	#	PKGS += libs_apps_uses_ssl

	# NB some of these also have repeats in -DVAR=val
	# as well as for makefile to zone stuff

	# Now compiling base2 WITHOUT GTK! SOON there will be no trace of GTK1
	# when that works, rework for the difference with GTK2

		WITH_GLIB=1
		WITH_GTK2=1

	# WITH_GTK=  ... 0=off 1=gtk-1.2 2=gtk-2.0
	# GTK1 probably no-longer works, but you could fix that

#		WITH_GTK=2
		WITH_GTK=0

	# You can use MYSQL client within LGPL as long as can switch to other db

		WITH_MYSQL=1
		WITH_PGSQL=0

	# POST is not working at the moment
	# maybe it needs an upgrade to 64 bit recent fixes
	# and it will happen, but the refactoring isnt quite right

	# PDFLIB is GPL # not for commercial products # (recompile off)
	# leave on for GPL complient usage, in same ELF, and same build tree
	# its a long time until PAGE_PRINT or lib_draw_cairo surfaces
	# writing PS_OUT to buffer through PS_2_PDF // with extra hooks
	# SPOUT_code_generator: include fragment
	# SPOUT_code_generator: push_scope_matrix(X)
	# SPOUT_code_generator: setup_pallette
	# SPOUT_code_generator: setup_pallette

		WITH_PDFLIB=1
	#	WITH_PDFLIB=0

		WITH_SPOUT=1
	#	WITH_SPOUT=0

	# there are some missing functions, but it runs
	# but some things try to build without it
	#	WITH_JS_SM=1
		WITH_JS_SM=0
		WITH_JS=0
	# if you change this - recompile everything
	# as obj_ref is derived from PyObject
	# this is not started, yet ...
		WITH_PYTHON=0
#		WITH_PYTHON=1

#
# CFLAGS -O2 -g
#

	# OPTIMISATION, architecture, CFLAGS
	# dwarf-4 (new default) needs gdb7.4 which insoght doesnt have
	OPTIM +=-g
# gcc	OPTIM +=-gstabs
	# OPTIM +=-O2
	# OPTIM +=-O3 # inline makes gdb crazy
	# OPTIM += -pg # profiling code
	# OPTIM +=-shared
	OPTIM += $(WARNS)
	# LD_SHARED_OPTS +=
	WARNS += -Wwrite-strings
	WARNS += -Wall
	# then switch some warings OFF with -Wno-XXX
	WARNS += -Wno-misleading-indentation

	# extra WARNS

	# OPTIM += -g

	# move to static_cast or const_cast
	# static_cast = zero runtime overhead, or PTR +- 4 bytes
	# dynamic_cast = check RTTI, must by VTBL class in source!
	# reinterpret_cast = static_cast but not PTR adjust - as if done
	# const_cast = to remove const 
	# char * to const unsigned char * needs a small paragraph
	#
#	WARNS += -Wold-style-cast

#
# X-BUILD XGCC WIN_AR - and also reset some WITH_xxx flags
#

### include $M/Makefile_X


#
# $(makes) = list of Makefiles to include soon
#
	# if any make file changes, relink the executables (extra.o)

	makes += $M/Make_Targets

	makes += $M/VAR_OBJS_base1
	makes += $M/VAR_OBJS_base1_glib
	makes += $M/VAR_OBJS_base2
ifeq ($(WITH_GTK2),1)
	makes += $M/VAR_OBJS_base2_gtk
	makes += $M/VAR_OBJS_gtk_text
endif
	makes += $M/VAR_OBJS_accts
	makes += $M/VAR_OBJS_sar_plot
	makes += $M/VAR_OBJS_media
	makes += $M/VAR_OBJS_play
	makes += $M/VAR_OBJS_demo
	makes += $M/Make_Targets2
#	makes += $M/Make_Targets_install
#	makes += $M/Make_exports

#
# -D how I like things - but maybe devel not production
#

	DEFS += -DQUITBTN	# adds a big QUIT button, optional
	# the problem is G_DEPRECATED_FOR(f) says
	#  error: attribute "__deprecated__" does not take arguments
	# G_DEPRECATED_FOR(atk_component_get_layer)

	DEFS += -DGLIB_DISABLE_DEPRECATION_WARNINGS	# I hate GTK
	DEFS += -DGDK_PIXBUF_DISABLE_DEPRECATED
	DEFS += -DPANGO_DISABLE_DEPRECATED
	DEFS += -DATK_DISABLE_DEPRECATED

ifeq ($(WIN32),1)
	dotEXE=.exe			# 
	DLL=dll#	# needed before in _TGT_WIN32
# CROSS	RM_rf = del			# too many arguments - do manually
	RM_rf = rm -rf
	DEPS=$(O)/depswin32#		# GCC builds a source dependency tree
	DEPS=$(O)/.deps#		# X-build on unix - can use dot

##	makes += $M/MK_WIN32/Make_WIN32	# add some extra dirs (edit for your system)
#
###	INCLS+=$(foreach sub,$(SubDirs), -I $S/$(sub) )
#
else
	dotEXE =
	DLL=so#	# default
	RM_rf = rm -rf

	##
	## YOU MUST USE THE SAME PLACE $(O) in libs_apps/_run/mk
	##

	DEPS=$O/.depps#			#

# switching this OFF
# moving to pkg-config method
# with some adjs
###	makes += $M/Make_UNIX #		#  (edit for your system)
#	makes += $M/MK_HERE/Make_GTK_1
#	makes += $M/MK_HERE/Make_GTK_2
#	makes += $M/MK_HERE/Make_JS
#	makes += $M/MK_HERE/Make_MYSQL
#	makes += $M/MK_HERE/Make_PDFLIB
#	makes += $M/MK_HERE/Make_PGSQL
#	makes += $M/MK_HERE/Make_SSL
#	makes += $M/MK_HERE/Make_ALSO
#	makes += $M/MK_HERE/Make_PYTHON


####	LX11 += -L/tools/..../X11R6/lib -lX11 -lXmu
	LX11 += -lXmu

 # THIS IS NOW SET IN Makefile_X_LGX_intel

	# ccache is a 'transparent' wrap on gcc/g++ that makes its go
	# faster when all the .h .c files havent changed.
	# including doxygen changes ;-) but not line-numbers/blanks
	# http://ccache.samba.org/ftp/ccache/
	## CXX = ccache g++ #			#
	# CXX = g++ #			#
	CXX = say_do g++ -O2 -g -fPIC #			# WRONG but CURRENT
	# CXX = say_do g++ #			#
#	CXX = RUN4 g++ #			#
#	CXX = RUN4 I icpc -g -debug all #			#
	# GDB = /tools/2003/ccs_insight/bin/gdb
	# GDB = /tools/2001/ccs/bin/gdb
	# GDB = /tools/2000/ccs/bin/gdb
	# GDB = /tools/2000/ccs/bin/gdb -nw
	# GDB = INGDB=1 /tools/2000/ccs/bin/insight
	# SuSE is /usr/X11R6/bin/insight, try PATH
	# GDB = INGDB=1 insight --cd $(PWD)
	GDB = INGDB=1 T gdb -tui --cd $(PWD)
endif

# rpath is where linux/posix finds the shared libraries
# PFX/lib is also a good idea

##	RPATHS += -Wl,--rpath=$(ABS_O) 
##	RPATHS += -Wl,--rpath=$(prefix_one)/lib 
	RPATHS += -Wl,--rpath=$(ABS_TOP)/lib 
	RPATHS += -Wl,--rpath=. 
# this works with icpc
	# RPATHS += -Wl,--rpath=/opt/intel/composer_xe_2011_sp1.7.256/compiler/lib/intel64
	RPATHS += -Wl,--rpath=/home/gps/G/mosjz 
	RPATHS += -g 
##	RPATHS += -debug all
##	LD_RPATHS += -rpath=$(ABS_O)

	LIBS += $(RPATHS)

	# TRALING SPACE REQUIRED TRACER # makes +=xxx$(DEPS)yyy
###	makes += $(DEPS)
### OK but need to override when not in src

# each of these are added to -I include
# each of these are added to VPATH 
# each of these SHOULD have make run locally there !

	SubDirs += /usr/include

	SubDirs += apps/main
	SubDirs += lib_play/hash
ifeq ($(WITH_GTK2),1)
	SubDirs += lib_play/grid_gtk
	SubDirs += lib_accts/accts_gtk
endif
	SubDirs += lib_play/js_scan
	SubDirs += lib_accts/accts_js
	SubDirs += lib_accts/table
	SubDirs += lib_accts/reports
	SubDirs += lib_accts/reports_CH
	SubDirs += lib_accts/tbl_tree
	SubDirs += lib_play/cfg_demo
	SubDirs += lib_play/amix
	SubDirs += lib_play/psg_expr

	## SubDirs += lib_base/bits # // but moved to subdir
	## SubDirs += lib_base/STR0/bits # // but making lib_base/STR0 -> ...
	## SubDirs += lib_base/BYTE/bits # // gets import - merge in from peer
	SubDirs += lib_base/STR0/bits # // current
	SubDirs += lib_base/fd
	SubDirs += lib_inet/fd_http
	SubDirs += lib_inet/fd_ftp
#	SubDirs += lib_inet/fd_cddb
#	SubDirs += lib_inet/fd_sar
#	SubDirs += lib_inet/fd_smtp

	SubDirs += lib_base/buffer
	SubDirs += lib_base/strings
	SubDirs += lib_base/amnt
	SubDirs += lib_base/obj
	SubDirs += lib_base/app


	# buf_P0P0 fd_buf fd_sel_ssl # // but not the rest
	
	SubDirs += lib_base/fd_api_buf
##	SubDirs += lib_base/fd_sel_ssl ## gone
	SubDirs += lib_win32/fd_win32

# WITH_PYTHON # was here #

ifeq ($(WITH_GLIB),1)
	SubDirs += lib_base/glib_1
	SubDirs += lib_base2/glib_2
endif
ifeq ($(WITH_GTK2),1)
	SubDirs += lib_base2/gtk_1
	SubDirs += lib_base2/gtk_1b
	SubDirs += lib_base2/gtk_2
	SubDirs += lib_base2/gtk_text
endif

	SubDirs += lib_base2/ssl_1
#	SubDirs += lib_base2/ssl_2 # slow down! (ease up)
	SubDirs += lib_base2/ssl_CA1
	SubDirs += lib_base2/ssl_pw

	SubDirs += lib_base2/draw_x11
ifeq ($(WITH_PDFLIB),1)
	SubDirs += lib_base2/draw_pdf
	SubDirs += lib_accts/reports_pdf
else
	SubDirs += lib_accts/reports_pdf
	# NB without_pdf.cxx needs to be found #
endif
ifeq ($(WITH_SPOUT),1)
	SubDirs += lib_base2/draw_spout
endif

#	SubDirs += lib_base/draw_ps
	SubDirs += lib_base/hash
	SubDirs += lib_base/time
	SubDirs += lib_base/misc
	SubDirs += lib_base/dgb
	SubDirs += lib_base/vars
ifeq ($(WIN32),1)
	SubDirs += lib_base/win32
	# lots of stuff is scattered within each file
	# so need improved clarity what is where, why
	# also fd_win32 is in fd with fd_unix ...
	# but for now it is somewhere to work
	SubDirs += lib_base/unix / add it anyway
else
	SubDirs += lib_base/unix
	SubDirs += lib_base/win32
endif
	SubDirs += lib_base/io_csv
	SubDirs += lib_base/prog_file

ifeq ($(WITH_JS_SM),1)
	# see also -I and -L in Rules file
	SubDirs += lib_base2/js_sm
endif
ifeq ($(WITH_JS),1)
	# see also -I and -L in Rules file
	SubDirs += lib_base2/js_1
#	SubDirs += lib_base2/js_2
endif

	SubDirs += sar_data/post-152
	SubDirs += sar_data/pdb
	SubDirs += sar_data/mem
	SubDirs += sar_data/dset

	SubDirs += sar_data/mysql
	SubDirs += sar_data/pgsql

	SubDirs += sar_parse/tbl_collect
	SubDirs += sar_parse/sar_hpux
	SubDirs += sar_parse/vmstat_hp1020

	SubDirs += sar_plot/btn_layout
	SubDirs += sar_plot/graph
	SubDirs += sar_plot/sar_plot

# now compiling on WIN32 but switched off inside
# ifneq ($(WIN32),1)

ifeq ($(WITH_JS_SM),1)
	SubDirs += sar_tests/stats_sysinfo
	SubDirs += sar_tests/proc_partitions
	SubDirs += sar_tests/js_stats
endif
# endif


# now compiling on WIN32 but switched off inside
# lets see how far we get
# ifneq ($(WIN32),1)
	SubDirs += lib_media/rem_xawtv
	SubDirs += lib_media/mixer
	SubDirs += lib_media/radio
	SubDirs += lib_media/ir
	SubDirs += lib_media/cd_audio

	SubDirs += lib_media/tv_remote
# endif

	SubDirs += apps/text_view

	CPLUS_INCLUDE_PATH+=-I/usr/include:


	# export tells MAKE to export this as an ENV var
	# its used by g++ to find all #include dirs, cleaner than if -I
	export CPLUS_INCLUDE_PATH

#	You might have to manually touch MK/.deps_HOSTNAME (see $(DEPS) above)
#	Then run mk_deps to index the full dependency list

#	OBJS_lib11 += $(OBJS_base1)
##	OBJS_lib11 += $(patsubst WORD_PATTER,WORD_REPLACEMENT,$(OBJS_base1))

ifeq ($(WITH_GLIB),1)
	OBJS_base1 += $(OBJS_base1_glib)
endif
ifeq ($(WITH_GLIB),1)
	OBJS_base2 += $(OBJS_base2_glib)
endif
	OBJS_lib11 += $(OBJS_base1)
	OBJS_lib21 += $(OBJS_base2)

ifeq ($(WITH_GTK2),1)
	OBJS_lib23 += $(OBJS_base2_gtk)
	OBJS_lib33 += $(OBJS_gtk_text)
endif
	OBJS_lib34 += $(OBJS_media)
	OBJS_lib35 += $(OBJS_accts)
	OBJS_lib36 += $(OBJS_sar_plot)


	# play objects go into base ???
	OBJS_lib11 += $(OBJS_base1_play)

	# RAND SSL needs base2
	OBJS_base2 += $(OBJS_base2_demo)

##	PROG1=accts
##	PROG1b=accts_nwa
##	PROG2=sar_plot
##	PROG3=gtk_text
##	PROG4=sar_parse
##	PROG5=tv_rem
##	PROG6=js_many
##	PROG7=wclient
##	PROG8=play_code
##	PROG9=tv_vol

####	#### TEMP building without GTK - only js_many can stay!
####	#### TEMP building without GTK - really I dont understand why
####	#### TEMP building without GTK - really its not my fault

	LOTS2 += $(PROG1_EXE)
	LOTS2 += $(PROG2_EXE)
	LOTS2 += $(PROG3_EXE)
	LOTS2 += $(PROG4_EXE)
	LOTS2 += $(PROG5_EXE)
ifeq ($(WITH_JS_SM),19)
	LOTS += $(PROG6_EXE)
endif
	LOTS2 += $(PROG7_EXE)
	LOTS2 += $(PROG8_EXE)
	LOTS2 += $(PROG9_EXE)

	LIST_LIB_A += lib$(LIB11).a
ifeq ($(WITH_GTK2),1)
	LIST_LIB_A += lib$(LIB35).a
	LIST_LIB_SO += lib$(LIB35).$(DLL)
endif
	LIST_LIB_A += lib$(LIB36).a
	LIST_LIB_A += lib$(LIB33).a
	LIST_LIB_A += lib$(LIB34).a
	LIST_LIB_A += lib$(LIB21).a

	LIST_LIB_SO += lib$(LIB11).$(DLL)
	LIST_LIB_SO += lib$(LIB36).$(DLL)
	LIST_LIB_SO += lib$(LIB35).$(DLL)
	LIST_LIB_SO += lib$(LIB33).$(DLL)
	LIST_LIB_SO += lib$(LIB34).$(DLL)
	LIST_LIB_SO += lib$(LIB21).$(DLL)

include $(makes)
include $M/Makefile_Last

# Make_Targets2 specifies the default (first target found)
# default: test2

lots: $(LOTS)
lots: $(LOTS2)

libs: $(LIST_LIB_A) $(LIST_LIB_SO)

so_list: $(LIST_LIB_SO)

# THESE ARE UNUSED ??
# dll_1: $(DLL11)
# dll_2: $(DLL21)
# dll_3: $(DLL33)
# dll_4: $(DLL34)
# dll_5: $(DLL35)
# dll_0: $(DLL36) dll_1 dll_2 dll_3 dll_4 dll_5

check_L24:
	L24_must_be


