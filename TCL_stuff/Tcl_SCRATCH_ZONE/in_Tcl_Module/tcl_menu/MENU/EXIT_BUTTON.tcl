
proc EXIT_BUTTON {{w {}}} {
	set w [not_dot $w]
	set btn $w.exit_button 
	button $btn -text EXIT -command {exit}
	v_pack $btn
}

