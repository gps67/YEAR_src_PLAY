#!/usr/bin/env tclsh
# test timer, fetch page, report time, loop 50 times
# count lines to ensure no hidden errors

package require http

proc get1 {} {
	set url http://localhost/py/
	set url http://localhost/py/test.py
	set url http://127.0.0.1/HTDOCS/SNAKE/SUB1/SUB2/fake.html
	set ::token [::http::geturl $url ] ;# -channel stdout
	# upvar #0 $token state
	# puts $state(body) ;# contents of retrieved file
}


# get one and print it, just to check that it isnt 404, or whatever
get1
upvar #0 $::token state
puts $state(body) ;# contents of retrieved file

set n 125
while {$n>0} {
	incr n -1
	set str [time get1 10] ;# loop 10 # returns string iterations per second
	set t [lindex $str 0]
	# set t [expr $t/1000000.0]
	set hz [expr 1000000.0/$t]
	set out [format "%4.5f hz %s" $hz $str]
	upvar #0 $::token state
#	puts $state(body)
	set nln [llength [split $state(body) "\n"]]
	puts "nlines: $nln $out"
}

# 404: 10 lines, 250 Hz, .004 sec
# OK: 14 lines, 130 Hz, .007 sec
# ten times faster than a few years ago, 1400 CPU not 400 x3 x3
# getting lots of req.attributes: 747 lines, often 50 Hz, 
# -ish-

