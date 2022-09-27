# a single toplevel (already .)
# contains text_out
# no menubar

proc mk_tcl_less_menubar  {{w_top .}} {
	
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

proc w2_c_names_c_vs_mf {w2 {plus {}}} {
	# _c _vs _mf
	# foreach {w2_c w2_vs w2_mf} [w2_c_names_c_vs_mf $w2 list1] {}
	if { $plus == {} } {
	} else {
		append plus _
	}
	return [list ${w2}.${plus}c ${w2}.${plus}vs ${w2}.${plus}mf]
}

proc w2_c_resized { w2_c w2_mf } {

	tkwait visibility $w2_c
	$w2_c create window 10 10 -anchor nw -window $w2_mf
	# $w2_mf has changed size
	# surrounding canvas needs to follow
	# bbox #
	# scrollregion
#	update idletasks
	set bbox [$w2_c bbox all]
	set mf_w [winfo reqwidth $w2_mf]
	set mf_h [winfo reqheight $w2_mf]
	puts "# mf_w # $mf_w #"
	puts "# mf_h # $mf_h #"
	puts "# BBOX # $bbox #"
	$w2_c config -width $mf_w -height $mf_h
	$w2_c config -scrollregion $bbox
	# limits scroll of parent to bbox
	# so APPEARs to be 0 0 # but still 10 10 #
}

proc build_tcl_less {{w2 .w2}} {

	# w2_demo # $w2 #
	# create $w2 as a toplevel 
	catch {
		toplevel $w2
	}

	set X {} ; set Y {} ; set W {} ; set H {} 
	set X 400 ; set Y 400 ;
	set W 200 ; set H 600 ;
	wm_geometry_XYWH $w2 $X $Y $W $H
	wm_minsize_WH $w2 400 200 ;# H does grow but W doesnt
#	wm_always_in_front $w2
#	wm_full_screen $w2
	wm_raise $w2 ;# not wm command

	# put EXIT button at the top, ready packed
	EXIT_BUTTON $w2 
	# following left packed canvas overlaps above EXIT button
	# OPTION $w2.row1 # v_packed under EXIT # h_packed items inside

	# toplevel w2 has its own MENU_BAR
	mk_tcl_less_menubar  $w2

	# the ONLY way to make the long list of buttons scroll
	# is to double wrap it in a canvas (or text or ...)
	# once upon a time I had a subsystem with
	# _outer_packable_ and _inner_packable_
	# but it went the same way as # 
	# wait_more wait_less wait_here
	# in addition w2_ROW must be a ROW not a COLUMN

if 0 {
	set plus menu1 ;# can be {}
	foreach {
		w2_c
		w2_vs
		w2_mf
	} [w2_c_names_c_vs_mf $w2 $plus] {}

	canvas $w2_c -bg green -yscrollcommand [list $w2_vs set]
	scrollbar $w2_vs -orient vertical -command "PUTS $w2_c yview"
	frame $w2_mf 

	h_pack $w2_vs ;# scroll bar on the left 
	h_pack $w2_c ;# canvas packed at its own width
#	v_pack $w2_mf ;# mf is not packed it is added to canvas 

# DEL#	w2_demo_fill_btns_items $w2_mf
	w2_c_resized $w2_c $w2_mf
}

	# unused TESTS
	bind $w2_c <MouseWheel> {puts "BIND W %W x %x y %y b %b d %d D %D a %a S %S T %T "; }
	bind $w2_c <ButtonPress-1> {puts "BIND W X %x Y %y b %b"; }
	bind $w2_c <ButtonPress-2> {puts "BIND W X %x Y %y b %b"; }
	bind $w2_c <ButtonPress-3> {puts "BIND W X %x Y %y b %b"; }
	bind $w2_c <ButtonPress-4> {puts "BIND W X %x Y %y b %b"; }
	bind $w2_c <ButtonPress-5> {puts "BIND W X %x Y %y b %b"; }
	bind $w2_c <ButtonPress-3> {puts "BIND %W scan mark   %x %y"; %W scan mark   %x %y}
	bind $w2_c <B3-Motion>     {%W scan dragto %x %y 1}
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
proc main_tcl_less {} {
	#
	# caller needs to bootstrap the path
	# a few useful things in the global space
	# that pick up SITE APP space
	#
	# lappend auto_path .

	set w1 . ;# {} ;# .
	set w2 .w2

	# w1_demo_build .
	# WINDOW ONE .
	EXIT_BUTTON $w1 ;# see at_least_dot.tcl
	menubar_demo_build_menubar $w1
	wm_raise $w1 ;# not a wm command

## move this to the MENU item or other start button
## TODO default on app start ... call w2_build ?

#	mk_text_out_global $w1 ;# " .text_out %s.%s {} "text_out":w

	# this defaults to panel in .
	# write text to it with # text_out {} $TEXT
	mk_text_out_global

	text_out_ln - "## LOADING ##"
	update idletasks
	# above has w1 not w2, w1 has reasonable sizes w2 not
	# w1 has the text_out  panel
	# w2 might refer to a spun-down second NFS disk # n seconds delay

	# WINDOW TWO $w2
#	build_tcl_less $w2
#	w2_demo_build $w2
}

# menubar_demo_main # caller calls this

