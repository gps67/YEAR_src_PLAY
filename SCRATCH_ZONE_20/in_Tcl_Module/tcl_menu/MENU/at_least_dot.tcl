
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

	proc w_above {w_below} {
		if { $w_below == {.} } {
			return {}
		}
		return .w_below
	}

	proc w_tail {w_full_name} {
		puts "TODO"
		if { $w_full_name == {.} } {
			return {}
		}
		return .w_full_name
	}

