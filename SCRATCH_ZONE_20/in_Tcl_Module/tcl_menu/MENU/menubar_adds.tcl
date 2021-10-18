
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
