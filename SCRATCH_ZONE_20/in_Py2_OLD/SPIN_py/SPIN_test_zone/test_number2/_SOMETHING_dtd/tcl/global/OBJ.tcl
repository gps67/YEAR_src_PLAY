set OBJ_id_counter 0
namespace eval ::_OBJ_ {}
proc OBJ_new_id {} {
	set id "::_OBJ_::v[incr ::OBJ_id_counter]"
}
proc OBJ_field_set {objid field value} {
	set "${objid}($field)" $value
}
proc OBJ_field_get {objid field} {
	set "${objid}($field)" 
}
# need some sort of field is array_var - in script
# seems to need a separate ${objid}$field
proc OBJ_ary_varname {objid field} {
	return "${objid}_$field" 
}
proc OBJ_ary_set {objid field key value} {
	set "[OBJ_ary_varname $objid $field]($key)" $value
}
proc OBJ_ary_get {objid field key} {
	set "[OBJ_ary_varname $objid $field]($key)" 
}
proc xxx_OBJ_set_field {objid field value} {
	OBJ_field_set $objid $field $value
}
proc xxx_OBJ_get_field {objid field} {
	set "${objid}($field)" 
}
proc OBJ_del {objid} {
	unset ${objid}
	foreach var [info vars [OBJ_ary_varname $objid *]] {
		# LURK - LEAK - ${objid}_* - looks horrid
		unset $var
	}
}
proc OBJ_array_set {objid vals} {
	# set objid [lindex $args 0]
	# set vals [lrange $args 1 end]
# puts "llength vals [llength $vals] $vals"
	array set ${objid} $vals
}
proc OBJ_array_get {objid} {
	array get ${objid}
}
proc OBJ_get_into_vars {objid vars} {
	# defaults ?
	foreach var $vars {
		upvar $var v
		set v [OBJ_field_get $objid $var]
	}
}
proc OBJ_set_from_vars {objid vars} {
	# defaults ?
	foreach var $vars {
		upvar $var v
		OBJ_field_set $obj $var [set v]
	}
}
proc OBJ_field_lappend {objid field value} {
	lappend "${objid}($field)" $value
}
