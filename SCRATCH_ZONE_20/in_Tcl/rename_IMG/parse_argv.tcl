#!/usr/bin/env tclsh

set ZONE_NAME {}

proc macro_POP_tail_a {top tail} {
 uplevel "
	set $top  \[ lindex \$$tail 0 \]
	set $tail \[ lrange \$$tail 1 end \]
 "
}

proc macro_POP_tail {top tail} {
	upvar $top up_top
	upvar $tail up_tail
	set up_top  [ lindex $up_tail 0 ]
	set up_tail [ lrange $up_tail 1 end ]
}

proc parse_argv_main argv {
	global ZONE_NAME
	set ARGS {}
	# iterate over list reducing it each time
	set tail $argv
	while {{} != $tail} {
		macro_POP_tail arg tail
		# show step by step through argv
		# puts "arg $arg tail { $tail }"

		if { $arg == "--ZONE_NAME" } {
			macro_POP_tail val tail
			global ZONE_NAME
			set ZONE_NAME $val
			puts "# --ZONE_NAME $ZONE_NAME # requested"
		} else {
			lappend ARGS $arg
		}
	}
	puts "# ZONE_NAME $ZONE_NAME"
	puts "# ARGS $ARGS"
}

parse_argv_main $argv
