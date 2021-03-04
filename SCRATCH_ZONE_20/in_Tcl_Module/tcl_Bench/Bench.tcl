#!/tools/CTXT/tcl/bin/tclsh8.7
#!/usr/bin/env tcl_SCRIPT_FILE
#!/tools/CTXT/tcl/bin/tclsh8.7
#!/usr/bin/env tclsh

puts "HELLO"

package require Tk
set D1 /home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_Bench/lib
lappend auto_path $D1

package require tcl_Bench

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




