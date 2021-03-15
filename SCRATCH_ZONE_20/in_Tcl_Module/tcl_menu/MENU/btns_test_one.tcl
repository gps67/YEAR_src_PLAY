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
set btns $w1.m1 ;# btns, a frame, a menu of buttons
frame $btns
h_pack $btns

# text_out is under dot
#mk_text_out_global {} ;# " .text_out %s.%s {} "text_out":w

mk_text_out_global $w1 ;# " .text_out %s.%s {} "text_out":w

PATH_add_gvim 

btns_b_text_cmd_Exit $btns

# $btns configure -relief sunken

if {0} {
 btns_b_text_cmd $btns [mk_id] "Hello World" {
    text_out_ln - "Hello, world";
 }

 btns_b_text_cmd $btns [mk_id] "RUN wish menu_one.tcl" {
    text_out_ln - [ exec wish menu_one.tcl]
 }

 btns_b_text_cmd $btns [mk_id] "RUN wish wish_list.tcl" {
    text_out_ln - [ exec wish wish_list.tcl]
 }
}

btns_ITEM_view_jpg  $btns { myfile.jpg }
btns_ITEM_edit   $btns { wish_list.tcl }
btns_ITEM_edit   $btns { menu_one.tcl }
btns_ITEM_edit   $btns { MENU/menu_one.tcl }
btns_ITEM_wish   $btns { wish_list.tcl }
btns_ITEM_wish   $btns { menu_one.tcl }
btns_ITEM_wish   $btns { MENU/menu_one.tcl }
btns_ITEM_script $btns { puts stdout "hello" }
btns_ITEM_script $btns { text_out_ln - "Button Two"; }
btns_ITEM_exec   $btns { openvpn }
btns_ITEM_exec   $btns { openvpn VLAN5.cfg }
btns_ITEM_script $btns {

btns_ITEM_script $btns { puts stdout "hello" }
    puts stdout "Message"
    # and then
}

# Local Variables:
# mode: tcl
# End:
