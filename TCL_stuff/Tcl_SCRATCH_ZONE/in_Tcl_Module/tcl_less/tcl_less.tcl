#!/usr/bin/env tclsh
# tcl_less.tcl - copy stdin to a text panel

# hmmm # gdb -tui -p 1234 # complains NOT_A_TTY # errout !! #

# https://github.com/gps67/YEAR_src_PLAY
# then setup symb links so that /home/gps/ -> valid
# and this link is also valid (soz)
#
# PWD
# /home/gps/YEAR/src/GITS/YEAR_src_PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_less

	set YEAR_src_PLAY /home/gps/YEAR/src/PLAY

	lappend auto_path \
	$YEAR_src_PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_menu/MENU \
	;#


# this is supposed to be a fragment of a larger BENCH
# that is a window, with a menubar, a text panel ...
# but without the demo code in the other main function it was copied from
# text_out is exactly this function, but fed from any direction

	tcl_less::main_tcl_less 

# SCRATCHES #
# fconfigure $chan -blocking 0 ;# TEXT -encoding binary
# fileevent $fd readable { ... }
# fileevent $fd writeable { ... }
# text_out_ln - "Some text out from main"
#
# cat afile | ./tcl_less.tcl # fails # infinite loop ?
# cat - | ./tcl_less.tcl # works
# maybe delay, manually typing in text
# notice gets returns EOF_ERROR ?
