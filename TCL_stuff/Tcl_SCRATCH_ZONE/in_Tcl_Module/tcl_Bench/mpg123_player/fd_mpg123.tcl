# errm ectually it is mpg123 not mpg321 # 
	namespace import ::MK_ID::mk*

namespace eval fd_mpg123 {

	# example of SINGLETON objext
	# I would prefer to pass "mpg123_fd" as token for global_fd

	variable global_fd
	variable RE_at_P_line
	build_RE_s


	proc mpg123_open { { mpg123_progname {} } } {
		if { $mpg123_progname == {} } {
			set mpg123_progname /tools/CTXT/libr/bin/mpg123 
			set mpg123_progname mpg321 
			set mpg123_progname mpg123 
		}
		set cmd "$mpg123_progname -R fake.mp3" 
		set mpg123_fd [open "| $cmd &" w+b]
		fconfigure $mpg123_fd -blocking 0 
		fconfigure $mpg123_fd -buffering none
		set pid [pid $mpg123_fd] ;#unused here

		puts  "cmd $cmd"
		puts  "mpg123_fd $mpg123_fd"
		puts  "pid $pid"
		text_out_ln - "cmd $cmd"
		text_out_ln - "mpg123_fd $mpg123_fd"
		text_out_ln - "pid $pid"

		variable global_fd
		set global_fd $mpg123_fd
		fileevent $mpg123_fd readable [list ::fd_mpg123::mpg123_on_readable $mpg123_fd]
		return $mpg123_fd
	}

	proc mpg123_send {mpg123_fd line } {

		text_out_ln - "# SEND # $line"
		puts $mpg123_fd $line
		flush $mpg123_fd

	}

	proc mpg123_close {mpg123_fd} {
		close $mpg123_fd w
		flush $mpg123_fd
		# set mpg123_fd {}
	}

	proc mpg123_LOAD {mpg123_fd filename} {
		mpg123_send $mpg123_fd "LOAD $filename"
	}

	proc mpg123_JUMP {mpg123_fd frames} {
		mpg123_send $mpg123_fd "JUMP $frames"
	}

	proc mpg123_PAUSE {mpg123_fd} {
		mpg123_send $mpg123_fd "PAUSE"
	}

	proc mpg123_GAIN {mpg123_fd gain} {
		# percentage
		mpg123_send $mpg123_fd "GAIN $gain"
	}

	proc mpg123_QUIT {mpg123_fd} {
		mpg123_send $mpg123_fd "QUIT"
	}


	############ on_readable ############

	proc mpg123_on_readable { mpg123_fd } {
	#	text_out_ln - "mpg123 fd READABLE"

		if {[gets $mpg123_fd OUT] > 0} {
		#	text_out_ln - "{OUT} $OUT"
		#	text_out_ln -       "$OUT"
			PARSE_at_line $OUT
			set OUT "OUT: $OUT"
		} else {
			if {[ eof $mpg123_fd ]} {
				text_out - {## EOF TODO WAIT LESS ##}
				text_out_ln - {<<ADDED EOLN>>}
				update idletasks
			#	fileevent AUTO on close
			#	fileevent $mpg123_fd readable {}
				set t [close $mpg123_fd]
				set $mpg123_fd {}
			}
			# RTFM says an incomplete line returns -1
			text_out_ln - "read returned {$OUT} close returned '$t'"
		}
	#       update idletasks
	}


} ;# namespace eval
