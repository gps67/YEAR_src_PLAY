namespace eval RE_CODES {

 variable EXPR_for_RE 
 variable VARS_for_RE 

 namespace export *

proc WARN {msg} {
	puts stderr $msg
}

# every named RE has a return field name "YYYY" "MM" "DD"
# every named RE has an RE_str
#
# RE_ is the NAME of a phrase, eg "cident99" 
# RE_str is the NAME of the variable that holds the str
# error if doesnt exist
# eexcept whilst building it

proc xRE RE {
#	# no no no
#	upvar RE RE_name
#	uplevel {
#		set RE "$RE_name"
#		set RE_str ${RE}_str
#	}
}

proc RE_clear { RE_ } {
	variable  EXPR_for_RE
	variable  VARS_for_RE
	set  EXPR_for_RE($RE_) ""
	set  VARS_for_RE($RE_) {}
}
proc RE_append_VAR { RE_ VAR_name } {
	variable VARS_for_RE 
	append  VARS_for_RE($RE_) $VAR_name
}

proc RE_append { RE_ args } {
	variable  EXPR_for_RE
	foreach arg $args {
		puts "RE_append $RE_ $arg"
		append EXPR_for_RE($RE_) $arg
	}
}

proc RE_addvar { RE_ VAR_name args } {
	variable  EXPR_for_RE
	variable  VARS_for_RE
	RE_append_VAR  $RE_ $VAR_name
	RE_append   {(}   $RE_ $args {)}
}


proc get_RE_phrase {name} { ;# no cached # caller does that

	switch -exact $name {
		numeric {
			return {[0-9]+} 
		}
		cident99 {
			return {[A-Za-z][0-9A-Za-z_]*}
		}
		xFF {
			return {x[0-9A-Fa-f]+}
		}
	}
	puts stderr "expected cident99 got $name"
	return {.*}
}

proc get_RE_phrase_cached {name} { 
	variable  EXPR_for_RE
	variable  VARS_for_RE
	# variable FMT_for_RE # default width
	global RE_CACHED
	if { [catch  {
		set val $RE_CACHED($name)
		return $val
	}]} {
		set val [get_RE_phrase $name]
		set RE_CACHED($name) $val
	}
	return $val
}

proc build_RE_at_R_line {} {
	upvar RE_at_R_line RE
	upvar RE_clear RE
	set AT "@"
	RE_append RE $AT
	RE_append RE $AT

}

} ;# namespace


