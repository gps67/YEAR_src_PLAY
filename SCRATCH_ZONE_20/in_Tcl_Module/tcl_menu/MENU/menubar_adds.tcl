
namespace eval ::menubar_adds {

# its ok for items to be loaded later
 namespace export *
 namespace import ::MK_ID::mk*

 # multiple top windows can have menubars
 proc add_menubar {{w_top .} {barname barname}} {
 	set w_top [not_dot $w_top]
	set w_bar $w_top.$barname
	# put the dot back
 	set w_top [at_least_dot $w_top]
	$w_top configure -menu [menu $w_bar] 
	return $w_bar
 }

 proc add_menu_tear {w_bar name {text {}} {tear 1}} { 
 	if {$text == {-}} { set text {} }
 	if {$text == {}} { set text $name }
 	if {$name == {-}} { set name [mk_id name] }
	set name m_$name
	set w_menu $w_bar.$name
        $w_bar add cascade -label $text -menu [menu $w_menu -tearoff $tear] 
	return $w_menu
 }

 proc add_menu {w_bar name {text {}} {tear 0}} { 
 	# you can provide $text which is not a good name
	# use name == - # mk_id
	return [add_menu_tear $w_bar $name $text $tear]
 }

 proc add_item {w_menu name {cmd ""}} { 

#	if {![winfo exists $w_menu]} { 
#	  puts "ERROR '$w_menu' $name $cmd"
#	  set w_bar [w_above $w_menu]
#	  set w_menu [add_menu $w_bar NAME]
#	}

	if [regexp ^-+$ $name] { 
		$w_menu add separator 
	} elseif {[regexp {^\?(.+)} $name -> name]} {
		$w_menu add checkbutton -label $name -variable $cmd
	} else {
		$w_menu add command -label $name -comm $cmd
	} 

 }

} ;# namespace


#
# demo code
# demo build_menubar
# demo main_menubar
#
proc demo_build_menubar {{w_top .}} {
  namespace import ::menubar_adds::*

# OK my style is CMD PREFIX STEP # returning FULL_NAME == PREFIX.STEP
# TK style is CMD FULL_NAME

  set w_bar  [add_menubar   $w_top mbar]
  set w_File [add_menu      $w_bar - {File Menu}]
  set w_Edit [add_menu_tear $w_bar - Edit]
  set w_Opts [add_menu      $w_bar - Opts]
  set w_Help [add_menu      $w_bar - Help]

  add_item $w_File Open {text_out_ln - "opened\n"} 
  add_item $w_File Save {text_out_ln - "saved\n"} 
  add_item $w_File ---- 
  add_item $w_File Exit exit 
  add_item $w_Edit Cut {}
  add_item $w_Opts verbose ;# checkbutton, toggles the global variable "verbose"
  add_item $w_Help About {text_out_ln - help_about}
}

proc demo_main_menubar {} {
 lappend auto_path .

  EXIT_BUTTON
  demo_build_menubar .
  toplevel .w2
  demo_build_menubar .w2
  canvas .w2.c
  v_pack_big .w2.c
# EXIT_BUTTON .w2 ;# works but resizes to "EXIT"
  EXIT_BUTTON .w2 ;# works but resizes to "EXIT"
  mk_text_out_global
}

# demo_main_menubar
