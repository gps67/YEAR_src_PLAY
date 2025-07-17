# gps_two.tcl

	package provide gps_two 0.5
	package require gps_one 0.4-

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
# /tools/x86_64/tcl/lib/tcl_lib_gps_two/
# which is actually a link so that it remains in git
# 
# invoked using:
# tclsh9.0 # not the /usr/lib tcl, the one with this lib/subdir/
#
#	package require gps_two 0.4-
#

# pkg_mkIndex notices the effect of this line


# then we do the package actions
# declare any funcs vars

# in our case we use this as a centralised place 
# where the dirs that we use will be found

	# OPTION # set auto_path [linsert $auto_path 0 D1 D2 D3 ...]
	# OPTION append puts system libs in front of UDEFS
	#
	# auto_path += //zipfs:/lib/tcl/tcl_library
	# auto_path += //zipfs:/lib/tcl
	# auto_path += /tools/x86_64/tcl/lib
	#


# an unused proc to confirm found
#	# TEST
	proc x_show_path {} {
		puts "show_path - who calls this"
		global auto_path
		foreach step $auto_path {
			puts "# auto_path += $step"
		}
	}
