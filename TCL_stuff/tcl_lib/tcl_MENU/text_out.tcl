
# you can have any number of $w2.text_out (different w2)
# mk_text_out_global creates one in .
# text_out - "TEXT" ;# appends to it
# text_out_ln - "TEXT" ;# with added newline
# GLOBAL VAR $text_out_global

# write some TEXT to the default log transcript
# uses global $text_out_global 
# boils down to #
# # $text_widget  insert end  $text


# THIS LINE MAKES tcl_less disrupt gdb by printing it twice # 1 #
# you might want it to do that, to keep a log of error messages
# gdb just messes up the tui
# gdb should know about output from progs
# set_text_out_does_puts 0 # if that is what you want
# set_text_out_does_puts 1 # if that is what you want
set ::global_text_out_does_puts 0

proc text_out_get_text_widget { {text_widget {} } } {
	# single default output text widget "-" or {} or "default"
	if {$text_widget == "-" } { set text_widget {}} 
	if {$text_widget == "default" } { set text_widget {}} 
	if {$text_widget == {}} {
	    set text_widget $::text_out_global
	}
	return $text_widget 
}

proc text_out {text_widget text} {
	# convert "-" or {} to the global default
	set text_widget [ text_out_get_text_widget $text_widget ]
	# add text to widget
	$text_widget insert end $text
	# print text to stout # not stderr # 
	if $::global_text_out_does_puts {
		puts -nonewline $text
	}

	text_out_see_end $text_widget ;# see last line
}

proc text_out_see_end {text_widget } {
	$text_widget see end
}

proc set_text_out_does_puts {{yes 1}} {
	set ::global_text_out_does_puts $yes
}


#
# text_out $TEXT # then a trailing newline
#
proc text_out_ln { text_widget text} {
	text_out $text_widget "$text\n"
	# text_out $text_widget "\n"
}

#
# limit the number of lines in text widget to nnn
#
proc text_out_limit_lines { { text_widget {} }  {lim 2000} } {
	set text_widget [ text_out_get_text_widget $text_widget ]
	# always seems to be lim.0 point zero
	set nn [$text_widget index end]
	set hi [ expr int( $nn - $lim ) - 1.0 ]
	if { $hi <= 0 } return
	$text_widget delete 1.0 $hi
}

#
# create the text_out panel w1.panelname
#
proc mk_text_out { w1 {text_out text_out}} {
     # w1 is created # along with its scroll bars etc
     # w1 must be specified # see mk_text_out_global
     # w1.panelname has default 
     set w1_text_out $w1.$text_out
     set w1_scroll $w1_text_out.scroll ;# nested fails zero size
     set w1_scroll $w1.scroll ;# works, loses scrollbar at 75% width
     # fontname is not var
     # font create font_text -family Helvetica -size 14
     font create font_text -family Fixed -size 14 

	# -width 100 # for INFO("usage"); #
     text $w1_text_out \
	-yscrollcommand [list $w1_scroll set] \
	-setgrid 1 \
	-height 30 \
	-width 100 \
	-undo 1 \
	-autosep 1 \
	-font font_text \
	;#
     ttk::scrollbar $w1_scroll -command [list $w1_text_out yview]
     h_pack_big $w1_text_out ;# -expand yes -fill both
     h_pack $w1_scroll ;# -side right -fill y
     return $w1_text_out
}

set text_out_global .unset
proc mk_text_out_global { {w1 {}} } {
	# w1 defaults to primary window "." # but dot gets added
	if { $w1 == "dot" } { set w1 {} }
	if { $w1 == "." } { set w1 {} }
	if { $w1 == "-" } { set w1 {} }

	global text_out_global 
	set text_out_global [mk_text_out $w1] ;# " .m1 .w1.m1 "
}

