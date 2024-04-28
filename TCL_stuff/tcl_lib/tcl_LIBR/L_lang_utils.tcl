
# L_ and S_ take upvar varname var
# comes from lset 

proc L_remove_all {list_varname item} {
	upvar $list_varname list
	set list [lsearch -all -inline -not -exact $list $item]
}

proc L_remove_one {list_varname item} {
	upvar $list_varname list
	set pos [ lsearch -exact $list $item] 
	if { $pos == -1 } {
		puts "# WARN # L_remove_one $list_varname {$list} '$item' # ABSENT"
		return
	}
	# L_remove_pos
	set list [lremove $list $pos]
	# puts "# INFO # L_remove_one $list_varname {$list} '$item' # AFTER"
}

proc L_contains {list_varname item} {
	upvar $list_varname list
	if { -1 == [ lsearch -exact $list $item] } {
		return 0
	} else {
		return 1
	}
}

proc S_contains {text_varname substr} {
	upvar $text_varname text
	if { -1 == [ string first $substr $text] } {
		return 0
	} else {
		return 1
	}
}


