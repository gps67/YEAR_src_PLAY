#!/usr/bin/env tclsh
puts "---ONE---"
# load tcl_parser_library.so
puts "---TWO---"
# load tcl_parser_library.so Parser_library
load ./tcl_parser_library.so Parser_library
puts "---DONE---"


# you specify where tokeniser::new loads the object to
# but the module loads new into the tokeniser:: namespace

namespace eval tokeniser { }

namespace eval tclsplit {

	load tcl_parser_library.so Parser_library
if [catch {
	tokeniser::new tclsplit::parser1
} err]  {
	puts stderr "# Module loading message: $err"
}

	variable filenamed

proc do_split_file file {
	if ![file isfile $file] {return 0}
	puts stderr "# split_file $file"
	set dir1 [file dirname $file]
	set dname [file rootname [file tail $file]]
	variable filenames
	set fd [open $file r]
	set script [read $fd]
	close $fd
	set script [ tclsplit::parser1 parse_script $script ]
	foreach command $script {
		foreach {cmnt stmt words} $command {
		  set space .
		  set name ${dname}__noproc
		  set isproc 0
		  if {"proc"==[lindex $stmt 0]} {
			set isproc 1
			set name [lindex $stmt 1] ;# error if missing
			set pars [lindex $stmt 2] ;# parameters
			set e {^(.*::)([^:]+)$}
			if [regexp $e $name all space name] { }
		  }
		  # reply on /./ being OK in filenames
		  set filename $dir1/$dname/$space/$name
		  variable filenames
		  ##
		  ## DO / DONT write out any files
		  ##
		  set do_write 0
		  if $do_write {
			  file mkdir $dir1/$dname/$space/
			  if [info exists filenames($filename)] {
				set fd [open $filename a] ;# append
			  } else {
				set fd [open $filename w] ;# truncate
				set filenames($filename) 1
			  }
			  puts $fd $cmnt
			  puts $fd $stmt
			  close $fd
		  }
		  if $isproc {
			global proc_list
			set proc_list($space$name) $pars
		  }
		}
	}
}

} ; ## namespace ##


if {$argv=={}} {
	set testdir /home/gps/9907/tcl-acct/scripts
	set filelist [glob $testdir/*.tcl]
	set argv $filelist
	# set testfile /home/gps/9907/tcl-acct/scripts/base/db.tcl 
	# set argv [list $testfile]
}
# puts "argv {$argv}"
foreach file $argv {
	tclsplit::do_split_file $file
}
foreach {procname procargs} [array get proc_list] {
	puts "  $procname [string trim $procargs]"
}

