# gps_one.tcl

# package require gps_one
# then have auto_path etc for all local (gps_one) site projects

#
# found from
#
# pkgIndex.tcl # produced by:
# pkg_mkIndex -verbose . *.tcl
#
# tclIndex # produced by: # and is empty
# auto_mkindex . *.tcl 
#
# found from being in a subdir of
# /tools/x86_64/tcl/lib/
# /tools/x86_64/tcl/lib/tcl_lib_gps_one/
# which is actually a link so that it remains in git
# 
# invoked using:
# tclsh9.0 # not the /usr/lib tcl, the one with this lib/subdir/
#
#	package require gps_one 0.4-
#

# pkg_mkIndex notices the effect of package provide PKG_NAME PKG_VERN
# DECL MACRO PKG_VERN = "0.5"
# do the package actions
# declare any funcs vars
# notify pkg_mkIndex
#
# 	package provide  PKG_NAME PKG_VERN

	package provide gps_one 0.5

# do the package actions
# declare any funcs vars

	# MORE CODE HERE

# "gps_one" is my default global ENV for Tcl scripts
# each module will bring its own package, running inside gps_one
# gps_two could be a custom preparation zone for this box, this task, gps_two
# gps_two is to be renamed TCL_IMPORT_PACKAGE_TASK_NAME # SITE_NAME # %s_%s
# ABBR %s_%s %s two # ABBR %s_%s SITE item_namer_one # SITE = "%s" two = "two" # 
# BENCH item_namer_one SITE # SITE = "SITE"
# any_dir_name under tcl/lib
# /tools/x86_64/tcl/lib/tcl_lib_gps_one/
# pkg_mkIndex notices the effect of package provide %s 0.5 # DECL HERE %s #
# %s # PKG_NAME # gps_one #

# in our case we use this as a centralised place 
# where the dirs that we use will be found

	# OPTION # set auto_path [linsert $auto_path 0 D1 D2 D3 ...]
	# OPTION append puts system libs in front of UDEFS
	#
	# auto_path += //zipfs:/lib/tcl/tcl_library
	# auto_path += //zipfs:/lib/tcl
	# auto_path += /tools/x86_64/tcl/lib
	#


	set in_Tcl_Module $env(HOME)/YEAR/src/PLAY/TCL_stuff/Tcl_SCRATCH_ZONE/in_Tcl_Module
	set as_tcl_lib $env(HOME)/YEAR/src/PLAY/TCL_stuff/tcl_lib

	# path includes all local projects
	# so that the package is found
	lappend auto_path \
		/home/gps/tcl_lib_gps_one	\
		/home/gps/tcl_lib_gps_two	\
		$env(HOME)/tcl_lib		\
		$as_tcl_lib			\
		$as_tcl_lib/tcl_gps		\
		$as_tcl_lib/tcl_LIBR		\
		$as_tcl_lib/tcl_Bench		\
		$as_tcl_lib/tcl_MENU		\
		$in_Tcl_Module/tcl_Bench	\
		$in_Tcl_Module/tcl_Bench/lib	\
	;#

	# pkgPath is auto in _one # because tcl/lib
	lappend tcl_pkgPath \
		/home/gps/tcl_lib_gps_two	\
		$env(HOME)/tcl_lib		\
		$as_tcl_lib			\
		$as_tcl_lib/tcl_gps		\
		$as_tcl_lib/tcl_LIBR		\
		$as_tcl_lib/tcl_Bench		\
		$as_tcl_lib/tcl_MENU		\
		$in_Tcl_Module/tcl_Bench	\
		$in_Tcl_Module/tcl_Bench/lib	\
	;#

# RTFM #  lappend tcl_pkgPath $D1 


	unset in_Tcl_Module
	unset as_tcl_lib



# an unused proc to confirm found
#	# TEST
	proc say_hello_one {} {
		puts "say_hello_one found here"
		global auto_path
		foreach step $auto_path {
			puts "# auto_path += $step"
		}
	}
