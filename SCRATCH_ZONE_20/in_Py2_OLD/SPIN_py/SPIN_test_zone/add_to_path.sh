# shell include file (defines functions)
# I put this in /etc/profile.d otherwise
#
#	. add_to_path.sh
#	add_to_var PATH $HOME/bin3
#	pfx_to_var LD_LIBRARY_PATH $HOME/lib4
#
#	NB: prefix doesnt force it to the front if already in var
#	Should work with bash, ksh, and maybe with very trad bourne shells
#	NB: overwrites the $val variable

# this is for debugging only
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
	echo "add_to_PATH in $0" # deprecated
	add_to_var PATH "$1"
}

pfx_to_PATH()
{
	echo "pfx_to_PATH in $0" # deprecated
	pfx_to_var PATH "$1"
}

