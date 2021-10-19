
# ADD NAMESAPCE w2 # w2_demo goes to w2 # 
# ADD NAMESAPCE w2_demo # for now # AKA demo_w2

# w2_demo # ALIAS w2 # ALIAS demo_w2 #
#
# demo code { menubar_demo } _build_menubar
# demo build_menubar
# demo main_menubar
#
proc w2_demo_build_menubar {{w_top .}} {
	
	namespace import ::menubar_adds::*

	# OK my style is CMD PREFIX STEP # returning FULL_NAME == PREFIX.STEP
	# TK style is CMD FULL_NAME

	set w_bar  [add_menubar   $w_top mbar]

	set w_File [add_menu      $w_bar - {File Menu}]
	set w_W2   [add_menu      $w_bar - W2]
	set w_Edit [add_menu_tear $w_bar - Edit]
	set w_Opts [add_menu      $w_bar - Opts]
	set w_Help [add_menu      $w_bar - Help]

#	add_item $w_MENU Name {CMDS }
	add_item $w_File Open {text_out_ln - "opened\n"} 
	add_item $w_File Save {text_out_ln - "saved\n"} 
	add_item $w_File ---- 
	add_item $w_File Exit exit 
	add_item $w_Edit Cut {}
	add_item $w_Opts verbose ;# checkbutton, toggles the global variable "verbose"
	add_item $w_Help About {text_out_ln - help_about}

	add_item $w_W2 Open {text_out_ln - "W2\n"} 
}

proc w2_demo_fill_menu_items { w2 }  { 
 # return

	set pfx /tools/x86_64/tcl
	set man $pfx/share/man
	set man3 $pfx/share/man/man3

	set list [glob -tails -directory $man3 * ]
	set list [lsort $list]

	set btns $w2
	foreach name $list {
	  text_out_ln - $name
	  btns_b_text_text_troff_file $btns - $name $man3/$name
	}
}

proc btns_b_text_text_troff_file { btns b text troff_file } {
	  # what m does
	  set m_cmd "groff -c -Tlatin1 -mandoc $troff_file |less -R "
	  # except it needs to be made a list and appended to in str
	  # and also T sh -c '%s' etc
	  # see also expect tkterm ...
	  set m m

	  btns_b_text_cmd $btns - $text "exec T m $troff_file"
}

proc wm_always_in_front {{wtop .}} {
	wm attributes $wtop -topmost 1 ;# -fullscreen 1
#	wm attributes $wtop -topmost 1    -fullscreen 1
}

proc wm_full_screen {{wtop .}} {
	wm attributes $wtop -fullscreen 1
}

proc wm_full_screen_toggle {{wtop .}} {
	wm attributes $wtop -fullscreen \
	 ![wm attributes $wtop -fullscreen]
}

proc w2_demo_build {{w2 .w2}} {

	catch {
		toplevel $w2
	}
#	canvas $w2.c
#	v_pack_big $w2.c
	EXIT_BUTTON $w2 ;# works but resizes to "EXIT"

	
	# w2 has its own MENU_BAR
	w2_demo_build_menubar $w2
	w2_demo_fill_menu_items $w2
	

	wm attributes $w2 -topmost 1 
	set W {}
	set H {}
	set X {}
	set Y {}
	set W 200
	set H 600
	set X 400
	set Y 400
	wm geometry $w2 =200x$H+$X+$Y
#	wm attributes $w2 -topmost 1 -fullscreen 1
	wm_always_in_front $w2
	raise $w2 ;# not wm command
	# above does not work much
}

# above is w2 // below is demo use of w2 api
#
# build two top level windows (with same menubar)
#
#  NAME demo_w2_main
#  NAME w2_demo_main
#  NAME demo_main
#  NAME w2_main
#
proc w2_demo_main {} {
	#
	lappend auto_path .

	set w1 {} ;# .
	set w2 .w2


	# w1_demo_build .
	# WINDOW ONE .
	EXIT_BUTTON $w1 ;# see at_least_dot.tcl
	menubar_demo_build_menubar $w1
	raise $w1

## move this to the MENU item or other start button
## TODO default on app start ... call w2_build ?

#	mk_text_out_global $w1 ;# " .text_out %s.%s {} "text_out":w

	# this defaults to panel in .
	# write text to it with # text_out {} $TEXT
	mk_text_out_global

	# WINDOW TWO $w2
	w2_demo_build $w2
}

# menubar_demo_main # caller calls this

