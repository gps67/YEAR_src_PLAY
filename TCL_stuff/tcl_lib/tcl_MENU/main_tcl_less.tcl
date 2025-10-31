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
		chan configure stdin -blocking 0 
		chan configure stdin -translation binary
		# binary means CR appears in data
		# binary means badly formed UTF8 - eg random bytes are vis

		chan event stdin readable {
		# fileevent stdin readable { ;# SCRIPT # on fileevent readable }
 # error reading "stdin": invalid or incomplete multibyte or wide character
 #    while executing
 # "gets stdin line"
 # adding this catch stopped tcl_less from doing anything much
 # even updating its menu when mouse over
 # but running not in pipe was OK manually ??

 	#	if {[catch { SCRIPT } ret_var_name opts_var_name]} {
	#	  # SCRIPT_FAILED # catch can return 0 TCL_OK 1 TCL_ERROR ...
	#	  # ret_var_name set to MESSAGE
	#	  # opts_var_name #
	#	  #  -level 0 # RTFM
	#	  #  -code 0 # RTFM ? a SCRIPT command
	#	} # else { WORKED_FINE }
	# ifok	# # ret_var now contains SCRIPTS return VALUE
	# fail	# # ret_var now contains ERROR_MESSAGE

	# 0	TCL_OK CATCH returns ZERO no problems no catch done
	# 1	TCL_ERROR
	# 2	TCL_RETURN
	# 3	TCL_BREAK
	# 4	TCL_CONTINUE

	# 5	TCL_OK
	# 6	TCL_OK
	# 7	TCL_OK

		 if {[catch {
			if {[chan gets stdin line] >= 0} {
				text_out_ln - $line
				text_out_limit_lines - 9500
			} else {
				text_out_ln - "gets failed"
				# RTFM says an incomplete line returns -1
		#		text_out_ln - "gets returned -1"
		# above printed twice per line # 2 of 3 #
				if {[ chan eof stdin ]} {
					# cancel own fileevent subscription
					# fileevent stdin readable {}
					chan event stdin readable {}
					close stdin 
					text_out_ln - {## EOF TODO WAIT LESS ##}
				}
			}
		 } retvar evars]} {
		 	# catch returned non zero not TCL_OK 
			puts stderr "# FAIL # TCL_LESS # $retvar # $evars"
			text_out_ln - "# INFO # chan tell [chan tell stdin]"

			set E_CODE [dict get $evars -errorcode ]
			try {
			}

			text_out_ln - "E_CODE $E_CODE"
			# fileevent stdin readable {}
			if {[ chan eof stdin ]} {
				text_out_ln - {## EOF=Y TODO WAIT LESS ##}
				chan event stdin readable {}
				close stdin 
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


