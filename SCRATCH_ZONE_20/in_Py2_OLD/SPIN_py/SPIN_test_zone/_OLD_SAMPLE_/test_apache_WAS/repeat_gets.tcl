#!/usr/bin/env tclsh
# test timer, fetch page, report time, loop 50 times
# count lines to ensure no hidden errors

package require http

proc get1 {} {
	set url http://localhost/py/
	set url http://localhost/py/test.py
	set ::token [::http::geturl $url ] ;# -channel stdout
	# upvar #0 $token state
	# puts $state(body)
}

set n 125
while {$n>0} {
	incr n -1
	set str [time get1 1]
	set t [lindex $str 0]
	# set t [expr $t/1000000.0]
	set hz [expr 1000000.0/$t]
	set out [format "%4.5f hz %s" $hz $str]
	upvar #0 $::token state
#	puts $state(body)
	set nln [llength [split $state(body) "\n"]]
	puts "nlines: $nln $out"
}

# comes to about 0.6 sec total envolope
# where Py reports 0.45 inside envolope
# -ish-

