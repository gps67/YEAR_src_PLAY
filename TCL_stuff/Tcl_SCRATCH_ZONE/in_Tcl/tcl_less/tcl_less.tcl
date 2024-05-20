#!/usr/bin/env tclsh9.0
#!/usr/bin/env tclsh
# tcl_less.tcl - copy stdin to a text panel

        package require gps_one ;# 0.5

# hmmm # gdb -tui -p 1234 # complains NOT_A_TTY # errout !! #

# https://github.com/gps67/YEAR_src_PLAY
# then setup symb links so that /home/gps/ -> valid
# and this link is also valid (soz)
#
# PWD
# /home/gps/YEAR/src/GITS/YEAR_src_PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_less

if 0 {
	#
	# This mess moved to package require gps_one
	#
	# The script that contains # package provide %s
	# that script is run (once) on # package require %s
	# PLUS gps_one is a link 
	# /tools/x86_64/tcl/lib/tcl_lib_gps_one/ # ->  
	# any actual subdir will do, name does not match anything # all index
	# /tools/x86_64/tcl/lib/%s/ SUBDIR 


	set YEAR_src_PLAY /home/gps/YEAR/src/PLAY
	set MENU $YEAR_src_PLAY/TCL_stuff/Tcl_SCRATCH_ZONE/in_Tcl_Module/tcl_menu/MENU 
	set MENU $YEAR_src_PLAY/TCL_stuff/tcl_lib/tcl_MENU/


	lappend auto_path \
	 $MENU \
	;#
}


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
