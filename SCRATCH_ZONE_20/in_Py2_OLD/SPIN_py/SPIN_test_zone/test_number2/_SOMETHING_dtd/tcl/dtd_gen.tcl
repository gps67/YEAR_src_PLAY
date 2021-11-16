# put has to be global for now

proc put {args} {
	switch [llength $args] {
	1 { ::puts stdout [lindex $args 0] }
	2 { 
		set nonew [lindex $args 0] 
		set msg   [lindex $args 1] 
		# ::puts stderr "put '$nonew' '$msg'"
		# ::puts -nonewline stderr TEST
		# ::puts -nonewline stderr $msg
		::puts [lindex $args 0] stdout [lindex $args 1]
	}
	default error "puts $args"
	}
}

namespace eval dtd_gen {

	namespace export *

	variable elem_list {}

proc add_common_atts_1 {tag {bool 1}} {
	if {!$bool} {return}
	# dtd_element::add_new_attr $tag id cdata ""
}

proc add_common_atts_2 {tag {bool 1}} {
	if {!$bool} {return}
	add_common_atts_1 $tag
	dtd_element::add_new_attr $tag date cdata "2000-01-01"
	dtd_element::add_new_attr $tag dbid cdata ""
}

proc gen_elem_from_spec { list } {
	set list [uplevel 1 [list subst  $list]]
	array set v $list
	set name $v(name)
	set subslist $v(subslist)
	set tag [dtd_element::new_elem $name $subslist]
#
	set fns {
		add_common_atts_1 
		add_common_atts_2 
		dtd_element::infer_start_tag
		dtd_element::omit_end_tag
	}
	foreach fn $fns {
		set boolvar v($fn)
		if [info exists $boolvar] {
			$fn $tag [set $boolvar]
		}
	}
	# should gen also print its output ?
	variable elem_list
	lappend elem_list $tag
	# dtd_element::print1 $tag
	return $tag
}

proc gen_file {} {

	put {<!-- gen_file -->}
	variable elem_list
	foreach tag $elem_list {
		dtd_element::print1 $tag
	}
}
} ;# dtd_gen

