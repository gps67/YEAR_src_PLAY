#!/usr/bin/env wish

# I think tcl uses this ...
package require Tk
lappend auto_path [pwd]/MENU
lappend auto_path [pwd]

namespace import ::MK_ID::mk*


set w1 .w1
toplevel $w1
wm title $w1 "W1 Text Out"
wm title . "MENU dot MENU" ;# TODO dir_csr dir_code dir_scripts

# .m1 is a list of buttons inside the . toplevel
# .ll is a lousy 
set menu_m1 $w1.m1
set menu_dot .m1
# set menu_m1 $menu_dot
# KNOW this file uses menu_m1 as var menu = menu_m1;
frame $menu_m1
frame $menu_dot
h_pack $menu_m1
h_pack $menu_dot

# put the new menu options in DOT next to the main text_out
if {0} {
	# TEXT is supposed to go to W1 # MENU to DOT
	# immediate single use call # CUSTOM CTOR #
	 mk_text_out_global $w1 ;# " .m1 .w1.m1 "
} else {
	# TEXT to DOT # needs irregular {} not . 
	# immediate second use call # CUSTOM CTOR #
	mk_text_out_global {} ;# " .text_out %s.%s {} "text_out":w
	# TRIED ... close text close all
}


# unused # maybe mk_widget_name
### filename_from_text_tests


btns_b_text_cmd $menu_m1 - "Exit" {
    puts stdout "Goodbye destroy dot";
    destroy .
}
btns_b_text_cmd $menu_m1 - "Hello World" {
    text_out_ln - "Hello, world";
}

btns_b_text_cmd $menu_m1 - "RUN wish wish_list.tcl" {
    text_out_ln - [ exec wish wish_list.tcl]
}

btns_ITEM_exec   $menu_m1 { wish wish_list.tcl }
btns_ITEM_exec   $menu_m1 { wish MENU/menu_one.tcl }
btns_ITEM_exec   $menu_m1 { gvim MENU/menu_one.tcl }
btns_ITEM_exec   $menu_m1 { gvim wish_list.tcl }
btns_ITEM_script $menu_m1 { puts stdout "hello" }
btns_ITEM_script $menu_m1 { text_out_ln - "Button Two"; }
btns_ITEM_script $menu_m1 {
    puts stdout "Message"
}
# HERE #
btns_ITEM_exec   $menu_dot { wish wish_list.tcl }	;# OK
btns_ITEM_exec   $menu_dot { wish MENU/menu_one.tcl }	;# conflict "wish"
btns_ITEM_exec   $menu_dot { EDIT wish_list.tcl }
btns_ITEM_exec   $menu_dot { EDIT MENU/menu_one.tcl }


# Local Variables:
# mode: tcl
# End:
