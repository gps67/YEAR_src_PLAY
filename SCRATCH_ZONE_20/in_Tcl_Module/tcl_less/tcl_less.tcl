#!/usr/bin/env tclsh
# tcl_less.tcl - copy stdin to a text panel

lappend auto_path \
/home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_menu/MENU \
;#

proc tcl_less_setup_stdin {} {
	fconfigure stdin -blocking 0 ;# -encoding binary
	fileevent stdin readable {
		if {[gets stdin line] >= 0} {
			text_out_ln - $line
		} else {
			# RTFM says an incomplete line returns -1
			text_out_ln - "gets returned -1"
			if {[ eof stdin ]} {
				fileevent stdin readable {}
				close stdin 
				text_out_ln - {## EOF TODO WAIT LESS ##}
			}
		}
		update idletasks
	}
}


package require Tk
# this is supposed to be a fragment of a larger BENCH
# that is a window with a menubar a text panel ...
# but without the demo code in the other main function it was copied from
# text_out is exactly this function, but fed from any direction
main_tcl_less 
tcl_less_setup_stdin 

text_out_ln - "you have to press CTRL-C or EXIT"


# SCRATCHES #
# text_out_ln - "Some text out from main"
# fileevent $fd readable { ... }
# fileevent $fd writeable { ... }
# fconfigure $chan -blocking 0 -encoding binary
#
# cat afile | ./tcl_less.tcl # fails # infinite loop ?
# cat - | ./tcl_less.tcl # works
# maybe delay, manually typing in text
# notice EXIT button not update idle
# notice gets returns EOF_ERROR ?
