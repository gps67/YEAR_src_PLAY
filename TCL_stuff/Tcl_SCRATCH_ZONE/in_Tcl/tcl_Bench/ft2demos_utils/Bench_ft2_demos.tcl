#! /usr/bin/env tclsh9.0

# testing version requirements
# package require gps_one 0.3-0.6 ;# OK with 0.5
# package require gps_one 0.3-0.4 ;# OK FAIL with 0.5
# package require gps_one 0.6- ;# OK FAIL with 0.5
# package require gps_one 0.3- ;# OK with 0.5

package require gps_one 0.3- ;# sets auto_path
package require tcl_Bench
# namespace import ::tcl_Bench::*
::tcl_Bench::abc
package require tk

package require tcl_gps
namespace import ::tcl_gps::*




if 0 {
	puts "# tcl_version [set tcl_version]"
	say_hello_one
	say_hello_two
}

set font1 {Courier 14}
set font1 {Courier 14 bold}

set t .t
text $t -font $font1
pack $t -fill both -expand 1


set filename textfile.txt
set filename ./Bench_ft2_demos.tcl
set text [read_text_file $filename]
#	# set fd [open $filename]
#	# set text [read $fd]
#	# close $fd
#	# return $text

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
# $t configure -width $w
# puts [$t configure ]


