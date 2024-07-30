#!/usr/bin/env tclsh9


package require gps_one 

set BADLIST { -+()[]{}<>=!\"£$%^&*@#~;:?.,}

set F { française _ LinkedIn.pdf}


foreach F1 $argv {
	set F2 [ filename_from_text $F1 ]
	if { $F1 != $F2 } {
		exec </dev/tty >/dev/tty 2>/dev/tty ask_do mv $F1 $F2
		break
	}
}

if 0 {
	# for people who dont trust my scripting
	# tcl_extension # SPELLING #
	# NEAR STEP # MATCH SPELLING # 
	# MATCH EXACT # ...
	# MATCH_CLOSE # ... more sub layers
	# MATCH_LOOSE # BOTH ADJACENT TO NEAR HERE # that sort of LOOSE MATCH #
	#
	# without tcl_extension that_module that_feature
	# PLAIN TCL9 # OK some syntax that is not known in TCL8 # Tcl9 # 
	# PLAIN TCL8 # COMPILES RUNS TESTED # TCL8 # I MUST COMPLY #
	# PLAIN TCL9 # COMPILES RUNS TESTED # TCL9 # so upgrade #
	# babel # need a framework (babel) (SWIG) {PSG_%s} 
}

