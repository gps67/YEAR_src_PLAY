# btns_b.tcl
# btns_b_text_cmd		TEXT {SCRIPT}
# proc btns_b_text_cmd {
#	 btns		$btns_m2
#	 b		{} {.} {-} {btn_123} 
#	 text		"MENU TEXT"
#	 script		{ puts {eval $script} }
#	} {
#		button $btns.$b   \
#		-text $text \
#		-command $script
#	}

proc btns_b_text_cmd { btns b text script } {
	# script is ARGV # "a Tcl command" list with ARGV # wuth own exec
	# eval $script
	if { $b == "-" } { set b {} }
	if { $b == {} } { set b [mk_id btn] } ;# must be lower case
	set btns_b $btns.$b
	text_out_ln - "btns_b_text_cmd btns $btns b $b text '$text' {$script} "
	if 0 { text_out_ln - "
btns_b_text_cmd
  btns $btns
  b $b
 -text '$text'
 -command {$script} "
	}
#	button $btns_b -text $text -command $script
	button $btns_b \
	-text $text \
	-command $script \
	-relief sunken \
	-overrelief raised \
	-anchor w \
	;#

	v_pack $btns_b 
#	$btns_b configure -anchor w ;# OK 
	return $btns_b
}

proc btns_ITEM_script { btns ARGV } {
	set b [mk_id btn]
	set text "$ARGV"
	btns_b_text_cmd $btns $b $text $ARGV
}

proc btns_ITEM_exec { btns ARGV } {
	set b [mk_id btn]
	set text "LBL_$b"
	set text "LBL_$ARGV"
	set text "$ARGV"
	set ARGV [linsert $ARGV 0 exec]
	btns_b_text_cmd $btns $b $text $ARGV
}

proc btns_ITEM_view_jpg { btns ARGV } { ;# ARGV is LIST
	set b [G_counter btn]
	set text "VIEW_JPG $ARGV"
	set ARGV [linsert $ARGV 0 exec eog ]
	btns_b_text_cmd $btns $b $text $ARGV
}

proc btns_ITEM_edit { btns ARGV } {
	set gvim gvim
	set gvim vi
	set ARGV [linsert $ARGV 0 $gvim]
	btns_ITEM_exec $btns $ARGV
}

proc btns_ITEM_wish { btns ARGV } {
	set ARGV [linsert $ARGV 0 wish]
	btns_ITEM_exec $btns $ARGV
}

proc btns_b_text_cmd_Exit { btns_m1 } {
	btns_b_text_cmd $btns_m1 [mk_id] "Exit" {
	    puts stdout "Goodbye destroy dot";
	    destroy .
	}
}
