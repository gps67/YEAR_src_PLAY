#!/usr/bin/env tclsh

set ZONE_NAME {}

proc POP_ARG_ARGV {up_arg0 up_argv} {
	upvar $up_arg0 arg0
	upvar $up_argv argv_tail
	set N   [ llength $argv_tail ]
	if { $N < 1 } {
		return 0; # FALSE no more data
	}

	set arg0  [ lindex $argv_tail 0 ]
	set argv_tail [ lrange $argv_tail 1 end ]
	return 1; # true

	if { $N < 2 } {
		if { $N == 0 } {
			puts "GOT N $N"
			return 0; # false
		}
		if { $N == 1 } {
			puts "GOT N $N"
			return 0; # false
		}
	}
	puts "GOT N $N" ; # NOISE"
	set arg0  [ lindex $argv_tail 0 ]
	set argv_tail [ lrange $argv_tail 1 end ]
	return 1; # true
}

proc parse_argv_main tail_argv {
	# argv is parameter to func # same value as global argv
	# ALTERNATIVELY # proc parse_argv_main { argv } { SCRIPT }
	# OUTERMOST gets "{ %s }" # WRAPPED in CURLIES
	global ZONE_NAME
	set ARGS {}
	# iterate over list reducing it each time
	# so give it a rename as "tail_argv" whats left of it
#	set tail_argv $argv
	while {[ POP_ARG_ARGV arg tail_argv ]} {
		# show step by step through argv
		# puts "arg $arg tail_argv { $tail_argv }"

		if { $arg == "--ZONE_NAME" } {
			if {![ POP_ARG_ARGV arg_val tail_argv ]} {
				puts "# FAIL # MISSING ARG "
				break
			}
			# POP_ARG_ARGV arg_val tail_argv
			global ZONE_NAME
			set ZONE_NAME $arg_val
			puts "# --ZONE_NAME $ZONE_NAME # requested"
		} else {
			lappend ARGS $arg
		}
	 }
	
	puts "# ZONE_NAME $ZONE_NAME"
	puts "# ARGS { $ARGS }"
}

parse_argv_main $argv
