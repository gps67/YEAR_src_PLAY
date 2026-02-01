
proc show_var VARNAME {
	upvar $VARNAME VAR
	puts "# $VARNAME ="
	puts "#      $VAR"
	puts "#."
}

