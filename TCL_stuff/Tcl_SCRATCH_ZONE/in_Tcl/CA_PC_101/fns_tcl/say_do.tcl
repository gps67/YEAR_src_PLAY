
proc say_do {cmd} {
	# cmd is a list # ARGS
	# need to add exec as first arg and call eval on that
	puts "# RUN # $cmd"
#	set cmd2 [linsert $cmd 0 exec </dev/tty ] 
	set cmd2 [linsert $cmd 0 exec -ignorestderr </dev/tty ] 
	;# -ignorestderr ;# raises error on output
	;# -keepnewline ;# already trimmed
	;# -- ;# 
	;# <@ $fileid ;# already opened r

#	lappend cmd2 >/dev/tty 2>&1
	# TODO catch #
	set ret {}
	# catch { eval $cmd2 } ret 
	try {
		set output [eval $cmd2]
		set exit_code 0
		set trap_says {}
		set ret 1
	} trap CHILDSTATUS {output trap_says} {
		;# output was output PLUS added 
		;# "child process exited abnormally"
		;# trap_says is a dict of pairs (K Vr)

		set exit_code [ lindex [dict get $trap_says -errorcode] 2]
		set ret 0
	}

	puts "# exit_code  # $exit_code"
	puts "# output     # $output"
	puts "# trap_says  # $trap_says"
	puts "# RET # $ret"
	return $ret
}

if 0 {set junk {
	rework exec
	POSS
		WRAP.exe A.exe R G V 

		+= TTY
		+= to_original stdin stdout errout
		+= `backtick` # uname -n
		+= pipe to stdin, stdout to pipe, 
		+= INTR PID cpu usage mem $? ...

	;# NAFF tcl lxxxx should use varname ;# concat 
	;# VARNAME lappend lset
	;# VALUE linsert 
	;# CHECK lindex llength lsearch lsort lrange lreplace
	;# list concat string ?join?

# trap_says #
	-code 1
	-level 0
	-errorstack {
		INNER {exec </dev/tty false >/dev/tty 2>&1}
		CALL {say_do false}
		CALL {say_do false}
	}
	-errorcode {CHILDSTATUS 63082 1}
	-errorinfo {child process exited abnormally
	while ... }

}}

