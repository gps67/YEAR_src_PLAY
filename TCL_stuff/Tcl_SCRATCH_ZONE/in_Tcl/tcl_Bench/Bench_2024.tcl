#!/usr/bin/env tclsh9.0

set Module_comment {
	each items uses an ANON NAME eg "Module_comment"

		"Module"
		"Module_%s" ;# Module "Module" "Module_STR0" "Module_EA_SPELLING"
		// _NAME "EA" // found as leading SUBLEX in NAME of AVAR
		// _AVAR "AVAR" // represent CLASS and INST // PICK INST //

		"Module_%s" _comment
		"Module_comment"
		"Module_XFFFF" idx_Module
		"Module _NAME" Module
	
		"Module_ANON" _SELF ; // TYPE_t == "MODULE_as_OBJ_ITEM"
		API.OBJ_API // == "MODULE_as_OBJ_ITEM" "_API"
		// _api gets _%s treatment as TEMPLATE LAYOUT FOR EXPRS for API
		// _api uses a CXX CPU_WORD LOHI_ARRAY HILO_ADAPTER
		// _api acts as PRE_CALL_API ROM
		// we can use LIST_ADD_ITEM and ITEM_FROM_EXPR and _ROM
		// _ROM_api += ALIAS += AVAR += // RHYME , RESONATE , MATCH //
		// _api ALIAS SPEC AVAR EXPR MODULE ROM LIBR STO RAM VFS AQL
		// VFS SQL  OBJ_in_SQL_NBYTES_of_SPEC_as_NBYTES either _or
		// _UDEF is recommended to start with _UNDER note _DUNDER _CIDENT
		// 
	
	each item creates _FIELDS {
	 NAME_ONE ...
	 NAME_TWO ...

	 NAME { VALUE }
	 NAME { DECL }
	 NAME { AVAR } { SCRIPT } {
	  PRE_CALL {
	  	# using ARGV in Tcl 
		# using OBJV_api in Tcl_SCRIPTS
		# using FEATURE _NAME _CIDEN99
		# AUTO TEST # _CIDENT99 is CIDENT99 # 
		# using FEATURE VAR_NAME #
		# using FEATURE VAR_NAME VAR_TYPE DATA_SPEC AVAR # AVAR #
		# 
	  }
	 }
	}
	
}

#!/usr/bin/env tclsh9.0
#!/usr/bin/env tclsh
#!/tools/CTXT/tcl/bin/tclsh8.7
#!/usr/bin/env tcl_SCRIPT_FILE
#!/tools/CTXT/tcl/bin/tclsh8.7

# puts "HELLO"
# invoked as tclsh # upgrade to Tk # but no wish init #
# package require Tk
package require gps_one

# BOOTSTRAP # where all SITE packages are to be subdirs # 
set D1 /home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_Bench/lib
#lappend auto_path $D1
 lappend tcl_pkgPath $auto_path ;# testing
 set tcl_pkgPath $auto_path ;# testing
 lappend tcl_pkgPath $D1 ;# notice spelling not tcl_pkg_path
puts "tcl_pkgPath '$tcl_pkgPath'"

# package require tcl_Bench ;# defines the abc function #
# package require tcl_Bench ;# uncommented as following line
  package require tcl_Bench

puts "## BUG ## abc not found but is nearby #"
## abc ;# throws if above not setup 

# uncomment to test other code path # supposed to FAIL #
# lappend auto_path $D1/tcl_Bench
if { [catch { my_func } retval ] } {
	puts "# FAIL # catch says FAIL error '$retval'"
 } else {
	puts "# OK catch says OK retval '$retval'"
}
;# 
puts "# adding dir to auto_path"
lappend auto_path $D1/tcl_Bench
my_func ;# 

#
# $D1/tcl_Bench/ # tcl_bench is a subdir of a dir on auto_path # $D1
# nothing else in this file (currently) uses tcl_Bench # 
#
# 
# $D1/
#  tcl_Bench/ # 
#	mk_pkgIndex
#		pkg_mkIndex -verbose . *.tcl
#	# tcl souirces all the files, and detects package provide
#
#	/home/gps/YEAR/src/PLAY~h/
#	my_pkg_tcl_Bench.tcl
#		package provide tcl_Bench 0.1
#		proc abc {} { puts abc }
#	# pkg_mkIndex detects package provides
#	# pkg_mkIndex creates pkgIndex.tcl
#	# pkg_mkIndex sets up { source $dir/my_pkg_tcl_Bench.tcl } ;#  $dir known
#	# caller gets the top file sourced # but { $dir } unknown
#
#
#	pkgIndex.tcl
#	package ifneeded tcl_Bench 0.1 [
#		list source [
#			file join $dir my_pkg_tcl_Bench.tcl
#		]
#	]
#	# actually this is generated as 1 line
#
# TODO 	# $dir needs to be added to auto_path to get the other files !!
# TODO	# $dir auto_path # needed { auto_mkindex . *.tcl } did not tell package


if 0 {

# dummy code that did something 
# probably was thinking about /MENU/ move that here ?
# but need auto_path anyway !!

set font1 {Courier 12 bold}
set font1 {Courier 12}

set t .t
text $t -font $font1
pack $t -fill both -expand 1

set filename textfile.txt
set fd [open $filename]
set text [read $fd]
close $fd

if 0 { set text {

        1 + 2 + 3 + 4 +
        r l r l r - - -
        r - - l r - - -
        - - - - - L R L
}}
set lines [split $text "\n"]
set w 5
set h 0
foreach line $lines {
        incr h
        set w1 [string length $line]
        puts "LINE: $line"
        if {$w < $w1} { set w $w1 }
}

$t insert end ljhlkjhlkjhlkjhlkjh
$t insert end $text

# set h [winfo reqheight $t ]
puts [$t configure ]
$t configure -height $h

# char -vs- pixels
# set w [winfo reqwidth $t ]
puts "w $w"
puts "h $h"
# set w 60
set w [expr $w * 2] ;# why why why
$t configure -width $w
# puts [$t configure ]



} else {
	puts "# OK # exiting from wish"
	exit
}

