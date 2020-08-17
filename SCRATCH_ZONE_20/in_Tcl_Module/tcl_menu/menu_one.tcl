#!/usr/bin/env wish

package require Tk
lappend auto_path [pwd]

proc PATH_add_gvim {} {
	set DIR1 {C:/Program Files (x86)/Vim/vim82}
	append ::env(PATH) ";$DIR1"
	puts $::env(PATH) 
}
PATH_add_gvim 

set w1 .w1
set w1 .
set w1 {}
set w1 .w1 ;# // do something else with .
toplevel $w1
wm title $w1 "Text Out"
# wm title . "Text Out" # 

# .m1 is a list of buttons inside the . toplevel
set menu_m1 $w1.m1
frame $menu_m1
h_pack $menu_m1

# text_out is under dot
mk_text_out_global {} ;# " .text_out %s.%s {} "text_out":w

menu_id_text_cmd_Exit $menu_m1

menu_id_text_cmd $menu_m1 [mk_id] "Hello World" {
    text_out_ln - "Hello, world";
}

menu_id_text_cmd $menu_m1 [mk_id] "RUN wish menu_one.tcl" {
    text_out_ln - [ exec wish menu_one.tcl]
}

menu_id_text_cmd $menu_m1 [mk_id] "RUN wish wish_list.tcl" {
    text_out_ln - [ exec wish wish_list.tcl]
}

menu_ITEM_exec   $menu_m1 { wish wish_list.tcl }
menu_ITEM_script $menu_m1 { puts stdout "hello" }
menu_ITEM_script $menu_m1 { text_out_ln - "Button Two"; }
menu_ITEM_exec   $menu_m1 { openvpn }
menu_ITEM_exec   $menu_m1 { openvpn VLAN5.cfg }
menu_ITEM_script $menu_m1 {
    puts stdout "Message"
    # and then
}

# Local Variables:
# mode: tcl
# End:
