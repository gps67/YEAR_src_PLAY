# a single toplevel (already .)
# contains text_out
# not really tcl_less more tcl_text_from_stdin
# but the surrounding frames not the io handler
# no menubar # borrow another as menubar placeholder

# TODO refactor into this namespace and simplify
namespace eval ::tcl_less {

	namespace export *

	# build one top level window (with a menubar)
	#
	proc main_tcl_less {} {

		# this is tk text utility
		package require Tk

		# the main toplevel . already exists
		set w1 . ;# {} ;# . #

		# bring the toplevel window to the top # 
		wm_raise $w1 ;# not a wm command

		# a second toplevel is not happening here
	#	set w2 .w2

		# I like a big EXIT button at the top
		EXIT_BUTTON $w1 ;# see at_least_dot.tcl

		# the menubar is optional
		mk_menubar $w1

		# add a big text panel to w1, and use it for text_out
		mk_text_out_global $w1

		# create the event handler for data in
		setup_stdin_to_text_out 

		text_out_ln - "you have to press CTRL-C or EXIT"
		# if there is a slow loading of thousands of man page buttons
		# U wants a loading message and a GUI update now
		# we certainly have enough to make a user happy
		# text_out_ln - "## LOADING ##"
		update idletasks
	}

	proc setup_stdin_to_text_out {} {
		# not fantastic with CR to BOLN to overwrite
		fconfigure stdin -blocking 0 ;# -encoding binary
		fileevent stdin readable {
			if {[gets stdin line] >= 0} {
				text_out_ln - $line
				text_out_limit_lines - 25
			} else {
				# RTFM says an incomplete line returns -1
		#		text_out_ln - "gets returned -1"
		# above printed twice per line # 2 of 3 #
				if {[ eof stdin ]} {
					fileevent stdin readable {}
					close stdin 
					text_out_ln - {## EOF TODO WAIT LESS ##}
				}
			}
		#	update idletasks
		}
	}

	proc mk_menubar {{w_top .}} {
		
		namespace import ::menubar_adds::*

		# TK style is CMD FULL_NAME # "PREFIX.STEP" #
		# MY style is CMD PREFIX STEP # PREFIX is over frame 
		# CMD returns FULL_NAME == PREFIX.STEP # of inner frame

		set w_bar  [add_menubar   $w_top mbar]

		set w_File [add_menu      $w_bar - {File Menu}]
		set w_W2   [add_menu      $w_bar - W2]
		set w_Edit [add_menu_tear $w_bar - Edit]
		set w_Opts [add_menu      $w_bar - Opts]
		set w_Help [add_menu      $w_bar - Help]

	#	add_item $w_MENU Name {CMDS }
		add_item $w_File Open {text_out_ln - "opened\n"} 
		add_item $w_File Save {text_out_ln - "saved\n"} 
		add_item $w_File Clear { $::text_out_global delete 1.0 end }
		add_item $w_File ---- 
		add_item $w_File Exit exit 
		add_item $w_Edit Cut {}
		add_item $w_Opts verbose ;# checkbutton, toggles the global variable "verbose"
		add_item $w_Help About { text_out_ln - help_about }

		add_item $w_W2 Open {text_out_ln - "W2\n"} 
	}

} ;# namespace


