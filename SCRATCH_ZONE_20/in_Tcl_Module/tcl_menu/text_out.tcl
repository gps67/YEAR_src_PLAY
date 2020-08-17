
proc text_out {text_widget text} {
	# single default output text widget "-" or {} oe "default"
	if {$text_widget == "-" } { set text_widget {}} 
	if {$text_widget == "default" } { set text_widget {}} 
	if {$text_widget == {}} {
	    global text_out_global
	    set text_widget $text_out_global
	}
	# add text to widget
	$text_widget insert end $text
}

proc text_out_ln { text_widget text} {
	text_out $text_widget $text
	text_out $text_widget "\n"
}

proc mk_text_out { w1 } {
     set text_out $w1.text_out
     text $text_out \
	-yscrollcommand [list $w1.scroll set] \
	-setgrid 1 \
	-height 30 \
	-undo 1 \
	-autosep 1
     ttk::scrollbar $w1.scroll -command [list $text_out yview]
     h_pack_big $text_out ;# -expand yes -fill both
     h_pack $w1.scroll ;# -side right -fill y
     return $text_out
}

set text_out_global .unset
proc mk_text_out_global { w1 } {
	if { $w1 == "dot" } { set w1 {} }
	if { $w1 == "-" } { set w1 {} }

	global text_out_global 
	set text_out_global [mk_text_out $w1] ;# " .m1 .w1.m1 "
}

