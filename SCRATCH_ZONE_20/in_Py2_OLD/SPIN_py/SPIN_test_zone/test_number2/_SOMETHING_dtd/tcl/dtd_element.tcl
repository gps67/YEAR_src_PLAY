
namespace eval dtd_element {

	proc new_elem {name subslist} {
		set obj [OBJ_new_id]
		OBJ_field_set $obj name $name
		OBJ_field_set $obj subslist $subslist
		OBJ_array_set $obj {
			attr_list	{}
			infer_start_tag	0
			omit_end_tag	1
			map_name	{}
			is_PRE		0
		}
		omit_end_tag $obj 1
		return $obj
	}
	proc infer_start_tag {obj bool} {
		OBJ_field_set $obj infer_start_tag [expr 0||$bool]
	}
	proc omit_end_tag {obj bool} {
		OBJ_field_set $obj omit_end_tag [expr 0||$bool]
		OBJ_field_set $obj emit_end_tag [expr !$bool]
	}
	proc emit_end_tag {obj bool} {
		omit_end_tag [expr !$bool]
	}
	proc add_new_attr {obj name type data} {
		set attr [list $name $type $data]
		OBJ_field_lappend $obj attr_list $attr
	}
	proc print1 {obj} {
		print_map $obj
		OBJ_get_into_vars $obj {
			attr_list
			infer_start_tag
			omit_end_tag
			name
			map_name
			subslist
		}
		set attlist {}
		foreach attr $attr_list {
			foreach {aname type data} $attr {
				if {$data=={}} {
					set data {""}
				} else {
					set data "\"$data\""
				}
				set line [format "%10s %-7s %s" $aname $type $data]
				append attlist "\n  " $line 
			}
		}
		set t1 -
		set t2 -
		if $infer_start_tag {
			set t1 o
		}
		if $omit_end_tag {
			set t2 o
		}
		put "<!element $name $t1 $t2 ($subslist) >"
		if {$attlist !={} } {
			put "<!attlist $name $attlist >"
		}
		if {$map_name!={}} {
		put "<!usemap $map_name $name>"
		}
	}
	proc add_map_line {obj pattern macro_name} {
		OBJ_field_lappend $obj map_lines "\n\t\"$pattern\" $macro_name"
	}
	proc print_map {obj} {
		set name [OBJ_field_get $obj map_name]
		if {$name=={}} return
		set lines [OBJ_field_get $obj map_lines]
		put -nonewline "<!shortref $name"
		foreach line $lines {
			foreach {pattern macro} $line {
				put -nonewline "\n\t\"$pattern\" $macro"
			}
		}
		put ">"
	}
}
