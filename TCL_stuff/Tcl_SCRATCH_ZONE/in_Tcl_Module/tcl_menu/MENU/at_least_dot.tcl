
# filter_window # could also reduce ..W to .W

	proc at_least_dot {w} {
		if { $w == {} } {
			return .
		} else {
			return $w
		}
	}

	proc not_dot {w} {
		if { $w == {.} } {
			return {}
		} else {
			return $w
		}
	}

# TEST # TODO

	proc w_above {w_below} {
		if { $w_below == {.} } {
			return {.}
			return {}
		}
		set pos [string last . $w_below]
	#	puts "# pos $pos # above"
		if { $pos <= 0 } {
			return {.}
		}
		incr pos -1
		string range $w_below 0 $pos
	}

	proc w_tail {w_full_name} {
		if { $w_full_name == {.} } {
			return {}
		}
		set pos [string last . $w_full_name]
		incr pos
	#	puts "# pos $pos #"
		string range $w_full_name $pos end
	}

	proc w_test {} {
	  foreach w1 {
		.a.bb.ccc
		.a.bb
		.a
		.
		{}
		nowt
	  } {
		set w2 [w_above $w1]
		set w3 [w_tail $w1]
		puts "# TEST W1 $w1 W2 $w2 W3 $w3"
	  }
	}

#	# w_test
