# menu_id.tcl
# menu_id_text_cmd		TEXT {SCRIPT}
# proc menu_id_text_cmd {
#	 menu		$menu_m2
#	 id		{} {.} {-} {btn_123} 
#	 text		"MENU TEXT"
#	 script		{ puts {eval $script} }
#	} {
#		button $menu.$id   \
#		-text $text \
#		-command $script
#	}

proc menu_id_text_cmd { menu id text script } {
	# script is ARGV # "a Tcl command" list with ARGV # wuth own exec
	# eval $script
	if { $id == "-" } { set id {} }
	if { $id == {} } { set id [mk_id] }
	set menu_id $menu.$id
	text_out_ln - "menu_id_text_cmd menu $menu id $id text '$text' {$script} "
	if 0 { text_out_ln - "
menu_id_text_cmd
  menu $menu
  id $id
 -text '$text'
 -command {$script} "
	}
	button $menu_id -text $text -command $script
	v_pack $menu_id
	return $menu_id
}

proc menu_ITEM_script { menu ARGV } {
	set id [G_counter btn]
	set text "$ARGV"
	menu_id_text_cmd $menu $id $text $ARGV
}

proc menu_ITEM_exec { menu ARGV } {
	set id [G_counter btn]
	set text "LBL_$id"
	set text "LBL_$ARGV"
	set text "$ARGV"
	set ARGV [linsert $ARGV 0 exec]
	menu_id_text_cmd $menu $id $text $ARGV
}

proc menu_ITEM_view_jpg { menu ARGV } { ;# ARGV is LIST
	set id [G_counter btn]
	set text "VIEW_JPG $ARGV"
	set ARGV [linsert $ARGV 0 exec eog ]
	menu_id_text_cmd $menu $id $text $ARGV
}

proc menu_ITEM_edit { menu ARGV } {
	set gvim gvim
	set gvim vi
	set ARGV [linsert $ARGV 0 $gvim]
	menu_ITEM_exec $menu $ARGV
}

proc menu_ITEM_wish { menu ARGV } {
	set ARGV [linsert $ARGV 0 wish]
	menu_ITEM_exec $menu $ARGV
}

proc menu_id_text_cmd_Exit { menu_m1 } {
	menu_id_text_cmd $menu_m1 [mk_id] "Exit" {
	    puts stdout "Goodbye destroy dot";
	    destroy .
	}
}
