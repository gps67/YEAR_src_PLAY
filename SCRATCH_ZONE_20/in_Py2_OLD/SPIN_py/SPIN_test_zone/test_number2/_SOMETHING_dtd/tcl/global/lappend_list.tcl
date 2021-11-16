proc lappend_list {varname list} {
	upvar $varname var
	set v2 $var
	foreach item $list {
		lappend v2 $item
	}
	set var $v2
}
