# ! /bin/bash 
#
# probably ancient bourne shall basic standard
# useful bit if syntax to borrow
#

	# SHOW "$AVAR"
	show_var() {
		local AVAR_varname="${1:-VARNAME}"
	eval    local AVAR_value="\$$AVAR_varname"
		
		printf "# SHOW # %s = '%s'\n" "${AVAR_varname}" "${AVAR_value}"
	}


	EMPTY_one=""	# is set # to a zero size string
	EMPTY_two="${EMPTY_one:-ZERO}"

	# ABSENT was UNSET #
	ABSENT="${ABSENT:-UNSET}"


	show_var EMPTY_one
	show_var EMPTY_two
	show_var ABSENT

exit
	# SHOW # EMPTY_one = ''
	# SHOW # EMPTY_two = 'ZERO'
	# SHOW # ABSENT = 'UNSET'

	That shows that the BASH SYNTAX does the substitution BOTH TIMES
	EMPTY_two holds the substitution invokes and done because _one was ""

