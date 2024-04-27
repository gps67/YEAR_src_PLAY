
# tcl_gps # is a LIBR of utility functions # not tk
package provide tcl_gps 0.1

# a list of functions like this one

namespace eval tcl_gps { ;# 

	variable unused { initval }

	proc read_text_file { filename } {
		set fd [open $filename]
		set text [read $fd]
		close $fd
		return $text
	}

	namespace export *
}
