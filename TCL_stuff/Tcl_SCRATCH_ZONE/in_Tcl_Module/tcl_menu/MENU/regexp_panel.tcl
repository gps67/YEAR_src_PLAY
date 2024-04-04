#!/usr/bin/env wish

package require Tk
lappend auto_path [pwd]/MENU
lappend auto_path [pwd]


# create $w0 == pre done by wish as "."

	set w0 {} ;# the default main toplevel

# create $w1

	set w1 .w1
	toplevel $w1

# titles

	wm title [at_least_dot $w0] "W0 MENU dot MENU" ;# main window proper
	wm title [at_least_dot $w1] "W1 Text Out" ;# extra popup menu

# .m1 is a list of buttons inside the . toplevel
# .ll is a lousy 

	set menu_dot $w0.m0	;# window0 m0 ;# built on "."
	set menu_m1  $w1.m1	;# window1 m1

# menu's are frames with names
# create the widget

	frame $menu_m1
	frame $menu_dot

# pack the widget

	h_pack $menu_m1
	h_pack $menu_dot

# create the big text panel
# on w0 the main window
# $w0.text_out 
# global text_out_global == ".text_out" or ".w1.text_out"

	mk_text_out_global $w0 ;# " .m1 .w1.m1 "

# unused # test filename from text
# unused # maybe mk_widget_name
# it converts "some __ string" to "some_string" 
# for use in filename_partname
# it has examples of regexp reducing a word

	filename_from_text_run_some_tests

# # #
# # #
# # #

	menu_id_text_cmd $menu_m1 - "Exit" {
	    puts stdout "Goodbye destroy dot";
	    destroy .
	}
	menu_id_text_cmd $menu_m1 - "Hello World" {
	    text_out_ln - "Hello, world";
	}

	menu_id_text_cmd $menu_m1 - "RUN wish wish_list.tcl" {
	    text_out_ln - [ exec wish wish_list.tcl]
	}

	menu_ITEM_exec   $menu_m1 { wish wish_list.tcl }
	menu_ITEM_exec   $menu_m1 { wish MENU/menu_one.tcl }
	menu_ITEM_exec   $menu_m1 { gvim MENU/menu_one.tcl }
	menu_ITEM_exec   $menu_m1 { gvim wish_list.tcl }
	menu_ITEM_script $menu_m1 { puts stdout "hello" }
	menu_ITEM_script $menu_m1 { text_out_ln - "Button Two"; }
	menu_ITEM_script $menu_m1 {
	    puts stdout "Message"
	}

	# HERE #
	menu_ITEM_exec   $menu_dot { wish wish_list.tcl }	;# OK
	menu_ITEM_exec   $menu_dot { wish MENU/menu_one.tcl }	;# conflict "wish"
	menu_ITEM_exec   $menu_dot { EDIT wish_list.tcl }
	menu_ITEM_exec   $menu_dot { EDIT MENU/menu_one.tcl }

# Local Variables:
# mode: tcl
# End:
