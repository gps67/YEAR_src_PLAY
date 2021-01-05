
eko()
{
	:
}

var_contains()
{
	# $1 is PATH
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

add_to_var()
{
	var_contains "$1" "$2" || var_append "$1" "$2"
}

pfx_to_var()
{
	var_contains "$1" "$2" || var_prefix "$1" "$2"
}

add_to_PATH()
{
	echo "add_to_PATH in $0"
	add_to_var PATH "$1"
}

pfx_to_PATH()
{
	echo "pfx_to_PATH in $0"
	pfx_to_var PATH "$1"
}
