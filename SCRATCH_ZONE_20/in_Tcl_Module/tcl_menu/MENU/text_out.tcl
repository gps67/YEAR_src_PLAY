
# you can have any number of $w2.text_out (different w2)
# mk_text_out_global creates one in .
# text_out - "TEXT" ;# appends to it
# text_out_ln - "TEXT" ;# with added newline
# GLOBAL VAR $text_out_global

proc text_out {text_widget text} {
	# single default output text widget "-" or {} or "default"
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

proc mk_text_out { w1 {text_out text_out}} {
     set w1_text_out $w1.$text_out
     set w1_scroll $w1_text_out.scroll ;# nested fails zero size
     set w1_scroll $w1.scroll ;# works, loses scrollbar at 75% width
     text $w1_text_out \
	-yscrollcommand [list $w1_scroll set] \
	-setgrid 1 \
	-height 30 \
	-undo 1 \
	-autosep 1
     ttk::scrollbar $w1_scroll -command [list $w1_text_out yview]
     h_pack_big $w1_text_out ;# -expand yes -fill both
     h_pack $w1_scroll ;# -side right -fill y
     return $w1_text_out
}

set text_out_global .unset
proc mk_text_out_global { {w1 {}} } {
	if { $w1 == "dot" } { set w1 {} }
	if { $w1 == "-" } { set w1 {} }

	global text_out_global 
	set text_out_global [mk_text_out $w1] ;# " .m1 .w1.m1 "
}

