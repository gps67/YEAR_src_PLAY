#!/usr/bin/env tclsh

package require Tk

set font1 {Courier 12 bold}
set font1 {Courier 12}

set t .t
text $t -font $font1
pack $t -fill both -expand 1

## set pick text_is_{TEXT}
## set pick text_is_{SCRIPT}
## set pick text_is_{EVAL}
## set pick text_is_{ARGV}
## DECL inline /* autogen C++ EXPR */
## DECL inline SCRIPT ARGV

set filename textfile.txt
# load $text from filename.txt
set fd [open $filename]
set text [read $fd]
close $fd

if 1 { set text {

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
	# output to stdout not TTY or GUI
        puts "LINE: $line"
        if {$w < $w1} { set w $w1 }
}

# TODO FIND OUT WHY every second word kept
# FILTER REWRITE as "All the words here"
$t insert end "First Text at TOP a b c d e f"
$t insert end  "\n"
$t insert end First Text at TOP a b c d e f
$t insert end  "\n"
$t insert end  "All the words here"
$t insert end  "\n"
# copy input text to output text // widget text panel 
$t insert end $text
$t insert end  "# EOF #"

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



# TODO # mpg123 # TCL_TK_BENCH mp3_player
# TODO # mpg123 # CTRL_RADIO_PLAYER_FRONTPLATE # mp3_player # API # .dll
# -k n # skip n frames # fps #
# -n n # n frames then stop
# -d n # every n''th frame #
# -@ f # filelist = f
# -u a # HTTP authentication string
# -E f # equaliser data from file f
# TCL EXEC needs some face-palm # FORK_PID_EXEC
# TCL child PID # pre_EXEC_code #
# SIGNAL SIGUSR1 # stop_start # 
# SIGNAL SIGUSR2 # next_track #
# --title # set terminal title # to filename #
