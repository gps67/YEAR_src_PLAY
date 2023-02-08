
# item is .w1.m1 or .floating_menu
# wish knows parent is .w1 or . # REQUIRED HERE #
# v_pack item # vertical stack like a menu # you create FRAME
# h_pack item # horizontal row of adjacent panels
# _big # this item absorbs all available space, the main panel

proc v_pack {item}	{ pack $item -side top  -fill x }
proc h_pack {item}	{ pack $item -side left -fill y }
proc v_pack_big {item}	{ pack $item -side top  -fill both -expand y }
proc h_pack_big {item}	{ pack $item -side left -fill both -expand y }

# proc h_pack_big {item} { pack $item -side left -fill y -expand y }

# GENERIC widget
proc forget_all_packed_content {w} { #
	set old_list [pack content $w]
	if { $old_list != {} } {
		set del_list [linsert $old_list 0 pack forget ]
		eval $del_list
	}
}


