# needs cd then call
#!/usr/bin/make -f
# this SHOULD be run in $(TOP)/_obj by the calling script

	LIB0=base
	LIB1=accts_gtk
	LIB2=sar_plot_post
	LIB3=gtk_text
	LIB4=media
	LIB5=base2

# on WIN32 set this in the calling maker.bat file, or edit this file

	WIN32=0

# on twin UNIX - different compilers! # UNUSED?

	UNAME=$(shell uname -n)

# ObjectType multi-build environments (on same machine) # UNUSED?

	OT=cc
	OT=py
	OT=js

# sorry about this - full length paths here, in mk_x.sh and in pkg-config files

	#
	# the directory (ABOVE) where this tree is housed
	#

	ABS_OVER=/nfs/md7/GPS/2009/ftp_build_2009/

# remember # comments add a trailng space to the variable (fatal in dirs)

	# cooledit requires an absolute path, for jump-to-error
	# -or- relative to _run_/_build_ dir
	# sorry, that means you have to edit it

	# either-way, you must create simple-name links, such as:
	# libs_apps -> libs_apps-2005-01-31
	# mm3_scan -> mm3_scan-2004-04-30
	#

	# other projects include this makefile
	# which includes other makefiles
	# which need to find TOP
	# so make a rel symb-link, or abs

	# OVER=../..
	# TOP=..

ifeq (1,0)
	OVER=$(ABS_OVER)
	TOP=$(OVER)/libs_apps
else
	OVER=../..
	TOP=..
endif

	# rpath preferes absolute paths ...

	ABS_TOP=$(ABS_OVER)/libs_apps

	M=$(TOP)/MK#		# where the Makefiles are
	S=$(TOP)/src#		# where the source dirs are

	# NB some of these are -DWITH_GTK2 but that causes recompile
	# so most are only used in the Makefiles

	# WITH_GTK=  ... 0=off 1=gtk-1.2 2=gtk-2.0
	# GTK1 probably no-longer works, but you could fix that
	WITH_GTK=2

	WITH_MYSQL=0
	WITH_PGSQL=0
	WITH_PDFLIB=1
	# there are some missing functions, but it runs
	WITH_JS=1
	# if you change this - recompile everything
	# as obj_ref is derived from PyObject
	# this is not started, yet ...
	WITH_PYTHON=0


	# OPTIMISATION, architecture, CFLAGS
	OPTIM +=-g
	# OPTIM +=-O2
	# OPTIM +=-O3 # inline makes gdb crazy
	# OPTIM += -pg # profiling code
	OPTIM += -fPIC
	OPTIM += $(WARNS)
	WARNS += -Wwrite-strings
	WARNS += -Wall

	# extra WARNS

	# move to static_cast or const_cast
	# static_cast = zero runtime overhead, or PTR +- 4 bytes
	# dynamic_cast = check RTTI, must by VTBL class in source!
	# reinterpret_cast = static_cast but not PTR adjust - as if done
	# const_cast = to remove const 
	# char * to const unsigned char * needs a small paragraph
	#
#	WARNS += -Wold-style-cast

	# if any make file changes, relink the executables (extra.o)

	makes += $M/Make_Targets

	makes += $M/VAR_OBJS_base1
	makes += $M/VAR_OBJS_base2
	makes += $M/VAR_OBJS_accts
	makes += $M/VAR_OBJS_sar_plot
	makes += $M/VAR_OBJS_gtk_text
	makes += $M/VAR_OBJS_media
	makes += $M/VAR_OBJS_play
	makes += $M/Make_Targets2
	makes += $M/Make_exports

	DEFS += -DQUITBTN	# adds a big QUIT button, optional

ifeq ($(WIN32),1)
	dotEXE=.exe			# 
	RM_rf = del			# too many arguments - do manually

##	O=$(TOP)/_obj_win32#		# where the objects go
	O=/tmp/_obj_win32#		# where the objects go
	DEPS=$(O)/depswin32#		# GCC builds a source dependency tree

	makes += $M/Make_WIN32	# add some extra dirs (edit for your system)
	LX11 += 

	CXX = g++ #			# the GNU C++ compiler
##	CXX = c:\MinGW\bin\g++ #	# cheating when confused

##	GDB = /tools/2000/ccs_insight/bin/gdb
##	GDB = gdb
	GDB = insight

	OPTIM += -mno-cygwin #		# no UNIX emulation layer required
	OPTIM += -mms-bitfields	#	# the new MINGW option
	OPTIM += -DWIN32 #		# tell source to do any fixups
#
###	INCLS+=$(foreach sub,$(SubDirs), -I $S/$(sub) )
#
else
	dotEXE =
	RM_rf = rm -rf

	##
	## YOU MUST USE THE SAME PATH $(O) in libs_apps/_run/mk
	##

	O=$(TOP)/lib# temps and binaries outside src
	ABS_O=$(ABS_TOP)/lib#
	# O=$(TOP)/_obj_$(UNAME)_$(WITH_GTK)#	# where the temp binaries go
	# O=$(TOP)/_obj_$(OT)_$(UNAME)_$(WITH_GTK)#	# where the temp binaries go
	# O=/tmp/_obj_$(UNAME)_$(WITH_GTK)#	# where the temp binaries go
	# DEPS=$O/.deps_$(UNAME)#			#
	DEPS=$O/.deps#			#

##	makes += $M/Make_UNIX #		#  (edit for your system)
	makes += $M/MK_HERE/Make_GTK_1
	makes += $M/MK_HERE/Make_GTK_2
	makes += $M/MK_HERE/Make_JS
	makes += $M/MK_HERE/Make_MYSQL
	makes += $M/MK_HERE/Make_PDFLIB
	makes += $M/MK_HERE/Make_PGSQL
	makes += $M/MK_HERE/Make_SSL
	makes += $M/MK_HERE/Make_ALSO
	makes += $M/MK_HERE/Make_PYTHON


####	LX11 += -L/tools/2000/X11R6/lib -lX11 -lXmu

	# ccache is a 'transparent' wrap on gcc/g++ that makes its go
	# faster when all the .h .c files havent changed.
	# including doxygen changes ;-) but not line-numbers/blanks
	# http://ccache.samba.org/ftp/ccache/
	## CXX = ccache g++ #			#
	CXX = g++ #			#
	# GDB = /tools/2003/ccs_insight/bin/gdb
	# GDB = /tools/2001/ccs/bin/gdb
	# GDB = /tools/2000/ccs/bin/gdb
	# GDB = /tools/2000/ccs/bin/gdb -nw
	# GDB = INGDB=1 /tools/2000/ccs/bin/insight
	# SuSE is /usr/X11R6/bin/insight, try PATH
	GDB = INGDB=1 insight
endif

# rpath is where linux/posix finds the shared libraries
# PFX/lib is also a good idea

	RPATHS += -Wl,--rpath=$(ABS_O)
	LD_RPATHS += -rpath=$(ABS_O)

	LIBS += $(RPATHS)

	# TRALING SPACE REQUIRED TRACER # makes +=xxx$(DEPS)yyy
	makes += $(DEPS)

# each of these are added to -I include
# each of these are added to VPATH 
# each of these SHOULD have make run locally there !

	SubDirs += /usr/include

	SubDirs += apps/main
	SubDirs += lib_play/hash
	SubDirs += lib_play/grid_gtk
	SubDirs += lib_play/js_scan
	SubDirs += lib_accts/accts_js
	SubDirs += lib_accts/accts_gtk
	SubDirs += lib_accts/table
	SubDirs += lib_accts/reports
	SubDirs += lib_accts/tbl_tree

	SubDirs += lib_base/bits
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

	SubDirs += lib_base2/gtk_1
	SubDirs += lib_base2/gtk_2
	SubDirs += lib_base2/gtk_text

	SubDirs += lib_base2/draw_x11
ifeq ($(WITH_PDFLIB),1)
	SubDirs += lib_base2/draw_pdf
	SubDirs += lib_accts/reports_pdf
endif
#	SubDirs += lib_base/draw_ps
	SubDirs += lib_base/hash
	SubDirs += lib_base/time
	SubDirs += lib_base/unix
	SubDirs += lib_base/io_csv
	SubDirs += lib_base/prog_file

ifeq ($(WITH_JS),1)
	# see also -I and -L in Rules file
	SubDirs += lib_base2/js_1
#	SubDirs += lib_base2/js_2
endif

	SubDirs += sar_data/post-138
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

	SubDirs += sar_tests/stats_sysinfo
	SubDirs += sar_tests/proc_partitions
	SubDirs += sar_tests/js_stats


ifneq ($(WIN32),1)
	SubDirs += lib_media/rem_xawtv
	SubDirs += lib_media/mixer
	SubDirs += lib_media/radio
	SubDirs += lib_media/ir
	SubDirs += lib_media/cd_audio

	SubDirs += lib_media/tv_remote
endif

	SubDirs += apps/text_view

	SP=$(empty) $(empty)# SINGLE SPACE
	COLON=:# no  spaces

	CPLUS_INCLUDE_PATH+=-I/usr/include:

	VPATH+=$S$(foreach sub,$(SubDirs),:$S/$(sub))
#	INCLS+=$(foreach sub,$(SubDirs), -I $S/$(sub) )
#	makes+=$(foreach sub,$(SubDirs), $S/$(sub)/MAKE_INCL)
	CPLUS_INCLUDE_PATH+=$(subst $(SP),$(COLON),$(foreach sub,$(SubDirs),$S/$(sub)))


	# there might be a problem if no ' ' exists at all # single dir
	# PKG_CONFIG_PATH crossing several dirs
	# why doesnt make have VAR_OPTION SEP_COLON varname
	#
	PKG_CONFIG_PATH=$(subst $(SP),$(COLON),$(PKG_CONFIG_dirs))
	export PKG_CONFIG_PATH

	# export tells MAKE to export this as an ENV var
	# its used by g++ to find all #include dirs, cleaner than if -I
	export CPLUS_INCLUDE_PATH

#	You might have to manually touch MK/.deps_HOSTNAME (see $(DEPS) above)
#	Then run mk_deps to index the full dependency list

	OBJS_lib0 += $(OBJS_base1)
	OBJS_lib5 += $(OBJS_base2)
	OBJS_lib1 += $(OBJS_accts)
	OBJS_lib2 += $(OBJS_sar_plot)
	OBJS_lib3 += $(OBJS_gtk_text)
ifneq ($(WIN32),1)
	OBJS_lib4 += $(OBJS_media)
endif
	# play objects go into base ???
	OBJS_lib0 += $(OBJS_play)

	LOTS += $(PROG1_EXE)
	LOTS += $(PROG2_EXE)
	LOTS += $(PROG3_EXE)
	LOTS += $(PROG4_EXE)
	LOTS += $(PROG5_EXE)
	LOTS += $(PROG6_EXE)
	LOTS += $(PROG7_EXE)
#	LOTS += $(PROG8_EXE)
	LOTS += $(PROG9_EXE)

	LIST_LIB_A += lib$(LIB0).a
	LIST_LIB_A += lib$(LIB1).a
	LIST_LIB_A += lib$(LIB2).a
	LIST_LIB_A += lib$(LIB3).a
	LIST_LIB_A += lib$(LIB4).a
	LIST_LIB_A += lib$(LIB5).a

	LIST_LIB_SO += lib$(LIB0).so
	LIST_LIB_SO += lib$(LIB1).so
	LIST_LIB_SO += lib$(LIB2).so
	LIST_LIB_SO += lib$(LIB3).so
	LIST_LIB_SO += lib$(LIB4).so
	LIST_LIB_SO += lib$(LIB5).so

include $(makes)

# Make_Targets2 specifies the default (first target found)
# default: test2

lots: $(LOTS)

libs: $(LIST_LIB_A) $(LIST_LIB_SO)

so_list: $(LIST_LIB_SO)



