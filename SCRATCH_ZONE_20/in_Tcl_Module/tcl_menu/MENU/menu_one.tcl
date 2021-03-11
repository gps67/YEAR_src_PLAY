#!/usr/bin/env wish

package require Tk
lappend auto_path [pwd]


set w1 .w1
set w1 .w1 ;# // do something else with .
set w1 {}
set w1 .
if {$w1!={.}} {toplevel $w1}
wm title $w1 "LOG Text Out"
# wm title . "LOG Text Out" # 
if {$w1=={.}} {set w1 {}}

# .m1 is a list of buttons inside the . toplevel
set menu_m1 $w1.m1
frame $menu_m1
h_pack $menu_m1

# text_out is under dot
#mk_text_out_global {} ;# " .text_out %s.%s {} "text_out":w

mk_text_out_global $w1 ;# " .text_out %s.%s {} "text_out":w

PATH_add_gvim 

menu_id_text_cmd_Exit $menu_m1

if {0} {
 menu_id_text_cmd $menu_m1 [mk_id] "Hello World" {
    text_out_ln - "Hello, world";
 }

 menu_id_text_cmd $menu_m1 [mk_id] "RUN wish menu_one.tcl" {
    text_out_ln - [ exec wish menu_one.tcl]
 }

 menu_id_text_cmd $menu_m1 [mk_id] "RUN wish wish_list.tcl" {
    text_out_ln - [ exec wish wish_list.tcl]
 }
}

menu_ITEM_view_jpg  $menu_m1 { myfile.jpg }
menu_ITEM_edit   $menu_m1 { wish_list.tcl }
menu_ITEM_edit   $menu_m1 { menu_one.tcl }
menu_ITEM_edit   $menu_m1 { MENU/menu_one.tcl }
menu_ITEM_wish   $menu_m1 { wish_list.tcl }
menu_ITEM_wish   $menu_m1 { menu_one.tcl }
menu_ITEM_wish   $menu_m1 { MENU/menu_one.tcl }
menu_ITEM_script $menu_m1 { puts stdout "hello" }
menu_ITEM_script $menu_m1 { text_out_ln - "Button Two"; }
menu_ITEM_exec   $menu_m1 { openvpn }
menu_ITEM_exec   $menu_m1 { openvpn VLAN5.cfg }
menu_ITEM_script $menu_m1 {

menu_ITEM_script $menu_m1 { puts stdout "hello" }
    puts stdout "Message"
    # and then
}

# Local Variables:
# mode: tcl
# End:
