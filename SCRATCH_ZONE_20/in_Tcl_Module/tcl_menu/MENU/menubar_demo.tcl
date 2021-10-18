

#
# demo code { menubar_demo } _build_menubar
# demo build_menubar
# demo main_menubar
#
proc menubar_demo_build_menubar {{w_top .}} {
  namespace import ::menubar_adds::*

# OK my style is CMD PREFIX STEP # returning FULL_NAME == PREFIX.STEP
# TK style is CMD FULL_NAME

  set w_bar  [add_menubar   $w_top mbar]
  set w_File [add_menu      $w_bar - {File Menu}]
  set w_Edit [add_menu_tear $w_bar - Edit]
  set w_Opts [add_menu      $w_bar - Opts]
  set w_Help [add_menu      $w_bar - Help]

  add_item $w_File Open {text_out_ln - "opened\n"} 
  add_item $w_File Save {text_out_ln - "saved\n"} 
  add_item $w_File ---- 
  add_item $w_File Exit exit 
  add_item $w_File w2_demo_build {w2_demo_build .w2} ;# HARDCODED .w2
  add_item $w_Edit Cut {}
  add_item $w_Opts verbose ;# checkbutton, toggles the global variable "verbose"
  add_item $w_Help About {text_out_ln - help_about}
}

#
# build two top level windows (with same menubar)
#
proc menubar_demo_main {} {
#
 lappend auto_path .

# WINDOW ONE .
  EXIT_BUTTON
  menubar_demo_build_menubar .

# WINDOW TWO .w2
  toplevel .w2
  menubar_demo_build_menubar .w2
  canvas .w2.c
  v_pack_big .w2.c
# EXIT_BUTTON .w2 ;# works but resizes to "EXIT"
  EXIT_BUTTON .w2 ;# works but resizes to "EXIT"

# this defaults to panel in .
# write text to it with # text_out {} $TEXT
  mk_text_out_global
}

# menubar_demo_main # caller calls this

