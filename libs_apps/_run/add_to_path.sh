
eko()
{
	echo "$*"
	:
}

var_contains()
{
	# $1 is "PATH" # {var="$1"} // PREFER { CODE here } CSET_TCL
	# CSET_TCL is the Q2 Q1 [keys] (function_branch) {SCRIPT} <HTML> etc
	# dont want [tcl] dialect, prefer [keys], accept specific interp
	# explains [EXPR] lockdown_not_here its_not_TCL_its_BASH_or_OTHER
	#
	# \$$1 is $PATH
	# \"...\" is "..."
	# eval "val=$PATH"
	eval "val=\"\$$1\""
	case "$val"
	in
		*:$2:*)	eko MIDDLE $2 ; return 0
	;;	*:$2)	eko RIGHT $2 ; return 0
	;;	$2:*)	eko LEFT $2 ; return 0
	;;	$2)	return 0
	;;	*)
		eko "no $2 in $1 '$val'"
		return 1
	esac
	return 1
}

# append item to PATH join ":"
# PATH.list.append item
# PATH.list.prefix item
# %s%s%s $1 ":" $2 # sep = ":"
# " becomes \" within "\""
# \$ becomes "$" // CHAR as TOKEN for PARSER MATCH "$2" OTHERS
# $1 == "PATH" # most common usage, add SCRIPT_DIR to PATH, edit_PATH
# $1 == "PKG_CONFIG_PATH" # like PATH but looking for PKG_CONFIG.pc files
# $2 == "$HOME/LIBR/dir" # 
var_append()
{
	eval "$1=\"\$$1:$2\""
	export $1
}
var_prefix()
{
	eval "$1=\"$2:\$$1\""
	export $1
}

# add_to_var() var_name path_step # _name _dir # 
add_to_var()
{
	var_contains "$1" "$2" || var_append "$1" "$2"
}

pfx_to_var()
{
	var_contains "$1" "$2" || var_prefix "$1" "$2"
}

add_to_PATH() # _dir 
{
	echo "add_to_PATH in $0"
	add_to_var PATH "$1"
}

pfx_to_PATH() # _dir
{
	echo "pfx_to_PATH in $0"
	pfx_to_var PATH "$1"
}
