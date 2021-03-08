
proc old_say_do {cmd} {
	# cmd is a list # ARGS
	# need to add exec as first arg and call eval on that
	puts "# RUN # $cmd"
	set cmd2 [linsert $cmd 0 exec] ;# NAFF tcl lxxxx should use varname
	# TODO catch #
	set ret noret
	catch { eval $cmd2 } ret 
	puts "# RET # $ret"
	return ret
}

